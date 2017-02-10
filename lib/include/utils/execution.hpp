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


/**
 * @class execution_error
 * Exeception class thrown on erronous process execution.
 */
struct execution_error : std::runtime_error
{
    explicit execution_error(const std::string& message);
};


/**
 * @class result
 * Encapsulates the result of an executed process.
 */
struct result
{
    /**
     * Constructor for result
     * @param s Success indicates whether the executed program returned successful or not.
     * @param ec Error code the returned error code of the executed program.
     * @param p The pid of the executed program.
     */
    result(bool s, int ec, pid_t p) :
        success(s), exit_code(ec), pid(p) {}

    /**
     * Default Constructor.
     */
    result() = default;

    bool success = true;

    int exit_code = 0;

    pid_t pid = 0;
};


/**
 * @class descriptor
 * Encapsulate a file descriptor, in a RAII object.
 */
struct descriptor
{
    descriptor(int fd_nr, std::string fd_name, std::function<bool(std::string&)> const& cb) :
        name(fd_name), callback(cb), read(true), fd_(fd_nr) {}

    descriptor(int fd_nr, std::string fd_name, std::string buf) :
         name(fd_name), buffer(buf), read(false), fd_(fd_nr) {}

    /**
     * Destructor
     * Closes file descriptor if necessary.
     */
    ~descriptor() {  if (fd_ < 0) close(fd_); }

    /**
     * Close file descriptor.
     * Set closed file descriptor to -1 to prevent double closing in destructor.
     */
    void release() { close(fd_); fd_ = -1; }

    /**
     * Get file descriptor
     */
    int fd() const { return fd_; }

    const std::string name;
    std::string buffer;
    std::function<bool(std::string&)> callback;
    bool read;

    private:
    int fd_;
};


/**
 * Search for a executable program of a given name.
 * @param prog_name The name of the program.
 * @return Returns the absolute path of the program name.
 */
std::string which(const std::string& prog_name);


/**
 * Execute a program and process its outputs line by line.
 * @param program The program to be executed.
 * @param arguments The vector containing the arguments passed to the program.
 * @param stdout_callback The callback processing the standard output of the program line by line.
 * @param stderr_callback The callback processing the standard error of the program line by line.
 * @param environment The map of the process environment that should be inherited by the child process.
 * @param input The string passed to the standard input of the process.
 * @param timeout The timeout how long to wait unitl the process have to be responsive.
 * @return Boolean, true if program ends successful, false otherwise.
 */
bool for_each_line(const std::string& program,
                   const std::vector<std::string>& arguments,
                   std::function<bool(std::string&)> const &stdout_callback,
                   std::function<bool(std::string&)> const &stderr_callback,
                   const std::map<std::string, std::string>& environment,
                   const char* input = nullptr,
                   unsigned int timeout = 0);


/**
 * Execute a program and process its standard output line by line.
 * @param program The program to be executed.
 * @param arguments The vector conatining the arguments passed to the program.
 * @param stdout_callback The callback processing the standard output of the program line by line.
 * @return Boolean, true if program ends successful, false otherwise.
 */
bool for_each_line(const std::string& program,
                   const std::vector<std::string>& arguments,
                   std::function<bool(std::string&)> const &stdout_callback);


/**
 * Execute a program and process its outputs line by line.
 *
 * Do not call this function directly.
 * This is function is internal called by the 'for_each_line' variants use these.
 *
 * @param program The program to be executed.
 * @param arguments The vector containing the arguments passed to the program.
 * @param stdout_callback The callback processing the standard output of the program line by line.
 * @param stderr_callback The callback processing the standard error of the program line by line.
 * @param environment The map of the process environment that should be inherited by the child process.
 * @param input The string passed to the standard input of the process.
 * @param timeout The timeout how long to wait unitl the process have to be responsive.
 * @return Returns result object indicating success or failure of the executed program.
 * @see for_each_line
 * @see result
 */
result execute(const std::string& program,
               const std::vector<std::string>& arguments,
               std::function<bool(std::string&)> const &stdout_callback,
               std::function<bool(std::string&)> const &stderr_callback,
               const std::map<std::string, std::string>& environment,
               const char* input = nullptr,
               unsigned int timeout = 0);


}}} /* closing namespace adafs::utils::exec */

namespace environment {


/**
 * Utility function to get the PATH of the current environment.
 * @returns Returns a vector of strings each sting indicate one path.
 * @see which
 */
std::vector<std::string> path();

} /* closing namespace environment */

#endif /* __ADAFS_UTILS_EXECUTION_HPP__ */
