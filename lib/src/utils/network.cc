#include "utils.hpp"

#include <string>

extern "C" {
#include <arpa/inet.h>
}

namespace sysmap { namespace utils { namespace network {

    bool is_ip(const std::string& ip)
    {
        unsigned char buf[sizeof(struct in6_addr)];
        return (inet_pton(AF_INET, ip.c_str(), buf) || inet_pton(AF_INET6, ip.c_str(), buf));
    }

}}} /* closing namespace sysmap::utils::network */
