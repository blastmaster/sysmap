#include "utils.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace adafs { namespace utils { namespace file {

namespace fs = boost::filesystem;


bool for_each_line(const std::string& path, std::function<bool(const std::string&)> cb)
{
    std::ifstream in{path};
    if (!in) {
        utils::log::logging::error() << "[sysmap::utils] Error, no file: " << path << "\n";
        return false;
    }

    for (std::string line; getline(in, line);) {
        if (! cb(line)) {
            break;
        }
    }
    return true;
}

bool for_each_line(std::istream& in, std::function<bool(const std::string&)> cb)
{
    if (!in) {
        utils::log::logging::error() << "[sysmap::utils] Error, bad ifstream\n";
        return false;
    }

    for (std::string line; getline(in, line);) {
        if (! cb(line)) {
            break;
        }
    }
    return true;
}

void for_each_subdirectory(const std::string& path, std::function<bool(const std::string&)> cb)
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
}

void for_each_file(const std::string& path, std::function<bool(const std::string&)> cb)
{
    fs::path p{path};
    auto d_iter = fs::directory_iterator(p);
    for (const auto& it : d_iter) {
        if (it.status().type() != fs::regular_file) {
            continue;
        }
        if (! cb(it.path().string())) {
            break;
        }
    }
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


std::string trim_regex_match(const std::ssub_match& match)
{
    std::string str = match.str();
    boost::trim(str);
    return str;
}

}}} /* closing namespace adafs::utils::file */
