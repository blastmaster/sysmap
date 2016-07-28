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

namespace fs = boost::filesystem;

static
bool for_each_line(const std::string& path, std::function<bool(const std::string&)> cb)
{
    std::ifstream in{path};
    if (!in) {
        return false;
    }

    for (std::string line; getline(in, line);) {
        if (! cb(line)) {
            break;
        }
    }
    return true;
}

static
bool for_each_subdirectory(const std::string& path, std::function<bool(const std::string&)> cb)
{
    fs::path p{path};
    auto d_iter = fs::directory_iterator(p);
    for (const auto& it : d_iter) {
        if (it.status().type() != fs::directory_file) {
            continue;
        }
        if (! cb(it.path().string())) {
            break;
        }
    }
    return true;
}

std::string read(const std::string& path);

bool read(const std::string& path, std::string& contents);


std::string read(const std::string& path)
{
    std::string contents;
    if (! read(path, contents)) {
        return "";
    }
    return contents;
}


bool read(const std::string& path, std::string& contents)
{
    std::ifstream in {path};
    std::ostringstream buffer;
    if (!in) {
        return false;
    }
    buffer << in.rdbuf();
    contents = buffer.str();
    return true;
}


}} /* closing namespace adafs::utils */

#endif /* __ADAFS_UTILS_HPP__ */
