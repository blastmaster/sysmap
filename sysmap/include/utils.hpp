#ifndef __ADAFS_UTILS_HPP__
#define __ADAFS_UTILS_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace adafs {

namespace utils {

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

//TODO
//here we want a generator like approach!
//this is currently a WIP dummy!
//
// Generators might have the following characteristics
// * there is a blob of data: the local variables define a state
// * there is an init method
// * there is a "next" method
// * there is a way to signal termination
// see also https://stackoverflow.com/questions/9059187/equivalent-c-to-python-generator-pattern
struct splitter {
    std::istringstream m_sline;

    splitter(const std::string& s) : m_sline(s) {}

    std::string next()
    {
        std::string buf;
        m_sline >> buf;
        return buf;
    }
};


//FIXME
//this is some shitty code it does what it shoud but just once
//because of static istringstream!!!
static
std::string split_first(const std::string& str)
{
    static std::istringstream stream(str);
    std::string buf;
    stream >> buf;
    return buf;
}


} /* closing namespace utils */

} /* closing namespace adafs */

#endif /* __ADAFS_UTILS_HPP__ */
