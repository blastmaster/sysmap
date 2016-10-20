#include <algorithm>

#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "utils/execution.hpp"
#include "utils/log.hpp"


namespace adafs { namespace utils { namespace exec {

namespace fs = boost::filesystem;

execution_exception::execution_exception(const std::string& message) :
    std::runtime_error(message) {}


static bool is_executable(const char* filename)
{
    struct stat fs;
    if (stat(filename, &fs) != 0) {
        return false;
    }

    auto euid = geteuid();

    if (euid == 0) {
        return fs.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH);
    }

    if (fs.st_uid == euid) {
        return fs.st_mode & S_IXUSR;
    }

    if (fs.st_gid == getgid() || fs.st_gid == getegid()) {
        return fs.st_mode & S_IXGRP;
    }

    return fs.st_mode & S_IXOTH;
}

std::string which(const std::string& prog_name)
{
    fs::path p = prog_name;
    if (p.is_absolute() && is_executable(p.c_str())) {
        return prog_name;
    }

    auto paths = environment::path();
    for (auto& dir : paths) {
        fs::path p = fs::path(dir) / prog_name;
        if (is_executable(p.c_str())) {
            return p.string();
        }
    }

    return {};
}

static auto get_max_fd_limit()
{
    rlimit lim;
    if (getrlimit(RLIMIT_NOFILE, &lim) == 0) {
        return lim.rlim_cur;
    }

    return STDERR_FILENO + 1UL;
}

static void exec_child(int in, int out, int err, char const* program, char const** argv, char const** envp)
{
    // set process group
    if (setpgid(0, 0) == -1) {
        adafs::utils::log::logging::error() << "setpgid failed\n";
        return;
    }

    // copy stdin
    if (dup2(in, STDIN_FILENO) == -1) {
        adafs::utils::log::logging::error() << "dub2 STDIN failed\n";
        return;
    }

    // copy stdout
    if (dup2(out, STDOUT_FILENO) == -1) {
        adafs::utils::log::logging::error() << "dub2 STDOUT failed\n";
        return;
    }

    // copy stderr
    if (dup2(err, STDERR_FILENO) == -1) {
        adafs::utils::log::logging::error() << "dub2 STDERR failed\n";
        return;
    }

    // close all fd's above stderr!
    for(decltype(get_max_fd_limit()) i = (STDERR_FILENO + 1); i < get_max_fd_limit(); ++i) {
        close(i);
    }

    // execute the given program
    execve(program, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
}


static pid_t create_child(int in, int out, int err, char const* program, char const** argv, char const** envp)
{
    pid_t child = vfork();
    if (child < 0) {
        adafs::utils::log::logging::error() << "vfork failed\n";
        throw execution_exception("failed to fork child process.");
    }

    // if we the parent, return childs pid
    if (child != 0) {
        return child;
    }

    // exec the child process, this returns only on failure.
    exec_child(in, out, err, program, argv, envp);

    // If this is reached an error occurs, so exit child
    _exit(errno == 0 ? EXIT_FAILURE : errno);
    return -1;
}


static std::vector<char const*> prepare_args(const std::vector<std::string>& arguments, const std::string& program)
{
    std::vector<char const*> result;
    result.reserve(arguments.size() + 2); /* argv[0] = program name + terminating null */

    result.push_back(program.c_str());
    std::transform(arguments.begin(), arguments.end(), std::back_inserter(result),
            [](const std::string& s) { return s.c_str(); });

    result.push_back(nullptr); /* terminating null */
    return result;
}


static std::vector<char const*> prepare_env(const std::map<std::string, std::string>& envp)
{
    std::vector<char const*> result;
    result.reserve(envp.size());

    std::transform(envp.begin(), envp.end(), std::back_inserter(result),
            [](const std::pair<std::string, std::string>& entry) {
                std::string s { entry.first + "=" + entry.second };
                return s.c_str();
    });

    return result;
}

static void read_write_child(std::array<descriptor, 3>& fds, unsigned int timeout = 0)
{
    fd_set read_set, write_set;
    while (1) {
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        timeval t_out = {};
        t_out.tv_sec = timeout;
        int ndfs = -1;
        int count = 0;

        for (auto& d : fds) {
            if (d.fd() < 0) {
                continue;
            }
            FD_SET(d.fd(), d.read ? &read_set : &write_set);
            if (d.fd() > ndfs) {
                ndfs = d.fd();
            }
        }

        if (ndfs == -1) {
            // all descriptors closed return
            return;
        }

        int result = select(ndfs + 1, &read_set, &write_set, nullptr, &t_out);
        if (result < 0) {
            adafs::utils::log::logging::error() << "Error select failed\n";
            return;
        }
        else if (result == 0) {
            adafs::utils::log::logging::debug() << "Warning select timeout\n";
            continue;
        }
        else {
            for (auto& d : fds) {
                if (d.fd() ==  -1 || !FD_ISSET(d.fd(), d.read ? &read_set : &write_set)) {
                    continue;
                }
                if(d.read) {
                    d.buffer.resize(4096);
                    count = read(d.fd(), &d.buffer[0], d.buffer.size());
                }
                else {
                    count = write(d.fd(), d.buffer.c_str(), d.buffer.size());
                }

                if (count < 0) {
                    adafs::utils::log::logging::error() << "Error pipe i/o write failed\n";
                    return;
                }
                if (count == 0) {
                    adafs::utils::log::logging::debug() << "pipe has closed\n";
                    d.release();
                    continue;
                }
                if (d.read) {
                    d.buffer.resize(count);
                    if (!d.callback(d.buffer)) {
                        adafs::utils::log::logging::debug() << "callback exit\n";
                        return;
                    }
                }
                else {
                    // remove data from buffer when written
                    d.buffer.erase(0, count);
                }
            }
        }
    }
}


static bool process_line(const std::string& data, std::string& buffer,
                        std::function<bool(std::string&)> const &cb)
{
    // no data, nothing to do
    if (data.empty()) {
        return true;
    }

    auto last_newline = data.find_last_of("\n");
    if (last_newline == std::string::npos) {
        // buffer data until we found a new line
        buffer.append(data);
        return true;
    }

    auto range = std::make_pair(data.begin(), data.begin() + last_newline);
    auto line_iter = boost::make_iterator_range(
            boost::make_split_iterator(range, boost::token_finder(boost::is_any_of("\n"), boost::token_compress_on)),
            boost::split_iterator<std::string::const_iterator>());

    for (auto& line : line_iter) {
        // append line to buffer
        buffer.append(line.begin(), line.end());
        boost::trim(buffer);
        if (buffer.empty()) {
            continue;
        }

        // call the actual callback on that line
        bool finished = !cb(buffer);

        // clear line for next iteration
        buffer.clear();

        if (finished) {
            return false;
        }
    }

    return true;
}


bool for_each_line(const std::string& program,
                   const std::vector<std::string>& arguments,
                   std::function<bool(std::string&)> const &stdout_callback,
                   std::function<bool(std::string&)> const &stderr_callback,
                   const std::map<std::string, std::string>& environment,
                   const char* input,
                   unsigned int timeout)
{
    std::string stdout_buffer;
    std::string stderr_buffer;

    auto new_stdout_cb = [&](std::string& s) {
        if (!process_line(s, stdout_buffer, stdout_callback)) {
            return false;
        }
        return true;
    };

    auto new_stderr_cb = [&](std::string& s) {
        if(!process_line(s, stderr_buffer, stderr_callback)) {
            return false;
        }
        return true;
    };

    return execute(program, arguments, new_stdout_cb, new_stderr_cb, environment, input, timeout).success;
}


result execute(const std::string& program,
               const std::vector<std::string>& arguments,
               std::function<bool(std::string&)> const &stdout_callback,
               std::function<bool(std::string&)> const &stderr_callback,
               const std::map<std::string, std::string>& environment,
               const char* input,
               unsigned int timeout)
{
    bool success = false;
    auto prog = which(program);
    if (prog.empty()) {
        const std::string msg = "Error " + program + " not in PATH.";
        throw execution_exception(msg);
    }

    int pipes[2];

    if (pipe(pipes) < 0) {
        adafs::utils::log::logging::error() << "Error due stdout pipe creation\n";
        throw execution_exception("failed to allocate stdout pipe.");
    }

    int stdout_read = pipes[0];
    int stdout_write = pipes[1];

    if (pipe(pipes) < 0) {
        adafs::utils::log::logging::error() << "Error due stdin pipe creation\n";
        throw execution_exception("failed to allocate stdin pipe.");
    }

    int stdin_read = pipes[0];
    int stdin_write = pipes[1];

    if (pipe(pipes) < 0) {
        adafs::utils::log::logging::error() << "Error due stderr pipe creation\n";
        throw execution_exception("failed to allocate stderr pipe.");
    }

    int stderr_read = pipes[0];
    int stderr_write = pipes[1];

    auto args = prepare_args(arguments, prog);
    auto envp = prepare_env(environment);

    auto child = create_child(stdin_read, stdout_write, stderr_write, prog.c_str(), args.data(), envp.data());

    // close file descriptors no longer need by parent process
    close(stdin_read);
    close(stdout_write);
    close(stderr_write);
    if (!input) {
        adafs::utils::log::logging::debug() << "[DEBUG] No input, closing stdin\n";
        close(stdin_write);
    }

    std::array<descriptor, 3> fds { { descriptor(stdout_read, "stdout", stdout_callback),
                                      descriptor(stderr_read, "stderr", stderr_callback),
                                      input ? descriptor(stdin_write, "stdin", input) : descriptor(-1, "", "") } };
    read_write_child(fds, timeout);

    int status = 0;
    if (waitpid(child, &status, 0) == -1) {
        adafs::utils::log::logging::error() << "waitpid failed\n";
    }
    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status);
        success = status == 0;
        adafs::utils::log::logging::debug() << "child exited normally: " << status << "\n";
    }
    if (WIFSIGNALED(status)) {
        status = WTERMSIG(status);
        adafs::utils::log::logging::debug() << "child terminated by signal: " << status << "\n";
    }

    adafs::utils::log::logging::debug() << "Result: " << (success ? "success " : "failed ")
        << " Exit code: " << status  << " child pid " << child << "\n";

    return {success, status, child};
}

}}} /* closing namespace adafs::utils::exec */


namespace environment {

std::vector<std::string> path()
{
    std::vector<std::string> result;
    std::string s_path(std::getenv("PATH"));
    boost::split(result, s_path, boost::is_any_of(":"), boost::token_compress_off);
    return result;
}

} /* closing namespace environment */
