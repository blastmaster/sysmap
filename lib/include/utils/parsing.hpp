#ifndef __SYSMAP_UTILS_PARSING_HPP__
#define __SYSMAP_UTILS_PARSING_HPP__

namespace sysmap { namespace utils {

    namespace parsing
    {
        /**
         * Checks if the given string contains an IPv4 Address
         * @Param ip string of ip
         * @Return Returns true if an IPv4 addr is found
         */
        bool contains_remote_address(const std::string& ip);

        bool is_remote_address(const std::string& ip);

        bool contains_hostname(const std::string& ip);

        bool is_hostname(const std::string& ip);
    }

}} /* closing namespace sysmap::utils */

#endif /* __SYSMAP_UTILS_LOG_HPP__ */
