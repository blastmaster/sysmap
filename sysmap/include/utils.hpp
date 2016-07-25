#ifndef __ADAFS_UTILS_HPP__
#define __ADAFS_UTILS_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

#include <boost/filesystem.hpp>

namespace adafs {

namespace utils {

namespace fs = boost::filesystem;

/**
 * utility function to split strings by an given delimiter
 * return the fields in a vector.
 */
static
std::vector<std::string>& split(const std::string& str, const char* delim, std::vector<std::string> &elems)
{
    std::istringstream stream(str);
    std::string item;

    while (getline(stream, item, *delim)) {
        elems.push_back(item);
    }

    return elems;
}

// OVERLOAD
static
std::vector<std::string> split(const std::string& str, const char* delim)
{
    std::vector<std::string> vec;
    return split(str, delim, vec);
}

// OVERLOAD
static
std::vector<std::string> split(const char* str, const char* delim)
{
    std::string tmp(str);
    return split(tmp, delim);
}

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


} /* closing namespace utils */

} /* closing namespace adafs */

#endif /* __ADAFS_UTILS_HPP__ */
