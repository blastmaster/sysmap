#include "utils.hpp"

namespace adafs { namespace utils {

namespace fs = boost::filesystem;


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


}} /* closing namespace adafs::utils */
