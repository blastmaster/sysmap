#ifndef __SYSMAP_UTILS_FILE_HPP__
#define __SYSMAP_UTILS_FILE_HPP__

#include <functional>
#include <regex>

namespace sysmap { namespace utils { namespace file {

/**
 * Execute a callback on each line of a given file.
 * @param path The path to the file.
 * @param cb The callback function that is passed each line in the file.
 * @return Returns true if file is successfully read, false otherwise.
 */
bool for_each_line(const std::string& path, std::function<bool(const std::string&)> cb);

bool for_each_line(std::istream& in, std::function<bool(const std::string&)> cb);

/**
 * Execute a callback on each file in a given directory.
 * @param path The path to the directory where traversal should start.
 * @param cb The callback that is passed each file in the directory.
 */
void for_each_subdirectory(const std::string& path, std::function<bool(const std::string&)> cb);

void for_each_file(const std::string& path, std::function<bool(const std::string& path, const std::string&)> cb);

/**
 * Utility function that Iterates over every possible substring of the given string str
 * and calls cb with each substr.
 * @param str String
 * @param cb Callback that get called with substring as argument
 * @return Returns true when cb returns true.
 */
bool for_each_substring(const std::string& str, std::function<const bool(const std::string&)> cb);

/**
 * Utility read function.
 * @param path The file that should be read.
 * @return Return the content was read.
 */
std::string read(const std::string& path);

/**
 * Utility function to read the content of a file in a string.
 * @param path The path to the file that should been read.
 * @param contents The string where to read the results.
 * @return Returns true when read was successful, false otherwise.
 */
bool read(const std::string& path, std::string& contents);

/**
 * Utility function to trim whitespaces from matches of regular expressions.
 * @param match The submatch of a regular expression.
 * @return Returns the string of the submatch without leading or trainling whitespaces.
 */
std::string trim_regex_match(const std::ssub_match& match);

}}} /* closing namespace sysmap::utils::file */

#endif /* __SYSMAP_UTILS_FILE_HPP__ */
