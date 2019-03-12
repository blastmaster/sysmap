#ifndef __SYSMAP_UTILS_NETWORK_HPP__
#define __SYSMAP_UTILS_NETWORK_HPP__

#include <string>

namespace sysmap { namespace utils { namespace network {

    /**
     * Checks if the given string represents a valid IPv4 or IPv6 address.
     * @param ip String which could represent an IP addr.
     * @return Returns true if the string is a valid IPv4 or IPv6 address.
     */
    bool is_ip(const std::string& ip);

}}} /* closing namespace sysmap::utils::network */

#endif /* __SYSMAP_UTILS_NETWORK_HPP__ */
