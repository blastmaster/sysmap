#ifndef __ADAFS_UTILS_EXECUTION_HPP__
#define __ADAFS_UTILS_EXECUTION_HPP__

#include <vector>
#include <map>
#include <string>
#include <functional>

extern "C" {
#include <unistd.h>
}

namespace adafs { namespace utils { namespace exec {

struct execution_exception : std::runtime_error {

    explicit execution_exception(const std::string& message);

};

struct result {

    result(bool s, int ec, pid_t p) :
        success(s), exit_code(ec), pid(p) {}

    result() {}

    bool success = true;

    int exit_code = 0;

    pid_t pid = 0;
};


struct descriptor {

    descriptor(int fd_nr, std::string fd_name, std::function<bool(std::string&)> const& cb) :
        name(fd_name), callback(cb), read(true), fd_(fd_nr) {}

    descriptor(int fd_nr, std::string fd_name, std::string buf) :
         name(fd_name), buffer(buf), read(false), fd_(fd_nr) {}

    ~descriptor() {  if (fd_ < 0) close(fd_); }

    void release() { close(fd_); fd_ = -1; }

    int fd() const { return fd_; }

    const std::string name;
    std::string buffer;
    std::function<bool(std::string&)> callback;
    bool read;

private:
    int fd_;
};


std::string which(const std::string& prog_name);

bool for_each_line(const std::string& program,
                   const std::vector<std::string>& arguments,
                   std::function<bool(std::string&)> const &stdout_callback,
                   std::function<bool(std::string&)> const &stderr_callback,
                   const std::map<std::string, std::string>& environment,
                   const char* input = nullptr,
                   unsigned int timeout = 0);

bool for_each_line(const std::string& program,
                   const std::vector<std::string>& arguments,
                   std::function<bool(std::string&)> const &stdout_callback);


result execute(const std::string& program,
               const std::vector<std::string>& arguments,
               std::function<bool(std::string&)> const &stdout_callback,
               std::function<bool(std::string&)> const &stderr_callback,
               const std::map<std::string, std::string>& environment,
               const char* input = nullptr,
               unsigned int timeout = 0);


}}} /* closing namespace adafs::utils::exec */

namespace environment {

std::vector<std::string> path();

} /* closing namespace environment */

#endif /* __ADAFS_UTILS_EXECUTION_HPP__ */
