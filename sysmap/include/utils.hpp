#ifndef __ADAFS_UTILS_HPP__
#define __ADAFS_UTILS_HPP__

#include <vector>
#include <string>
#include <iostream>

namespace adafs {

namespace utils {

/**
 * utility function to split strings by an given delimiter
 * return the fields in a vector.
 */
static
std::vector<std::string>& split(const std::string& str, const char* delim, std::vector<std::string> &elems)
{
    std::stringstream stream(str);
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


} /* closing namespace utils */

} /* closing namespace adafs */

#endif /* __ADAFS_UTILS_HPP__ */
