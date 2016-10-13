#ifndef __ADAFS_UTILS_FILE_HPP__
#define __ADAFS_UTILS_FILE_HPP__

#include <functional>
#include <regex>

namespace adafs { namespace utils { namespace file {


bool for_each_line(const std::string& path, std::function<bool(const std::string&)> cb);

void for_each_subdirectory(const std::string& path, std::function<bool(const std::string&)> cb);

std::string read(const std::string& path);

bool read(const std::string& path, std::string& contents);

std::string trim_regex_match(const std::ssub_match& match);


}}} /* closing namespace adafs::utils::file */

#endif /* __ADAFS_UTILS_FILE_HPP__ */
