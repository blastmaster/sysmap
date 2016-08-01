#ifndef __ADAFS_UTILS_HPP__
#define __ADAFS_UTILS_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

#include <boost/filesystem.hpp>

namespace adafs { namespace utils {


bool for_each_line(const std::string& path, std::function<bool(const std::string&)> cb);

void for_each_subdirectory(const std::string& path, std::function<bool(const std::string&)> cb);

std::string read(const std::string& path);

bool read(const std::string& path, std::string& contents);


}} /* closing namespace adafs::utils */

#endif /* __ADAFS_UTILS_HPP__ */
