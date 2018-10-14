#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils.hpp"
#include <arpa/inet.h>

auto is_ip = [](const std::string& ip){
    unsigned char buf[sizeof(struct in6_addr)];
    return (inet_pton(AF_INET, ip.c_str(), buf) || inet_pton(AF_INET6, ip.c_str(), buf));
};

TEST_CASE("Test if IPv4/IPv6 Addresses are recognized"){
    REQUIRE(sysmap::utils::file::for_each_substring("10.0.0.1", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("192.68.2.4", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("1.1.1.1:/mnt/foo", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("8.8.8.8", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("255.255.255.0", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("234.245.231.2", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("ThisStringContainsFoo234.245.231.2MoreFoo", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("ThisStringContainsFoo234.265.231.2MoreFoo", is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("6.266.1.1", is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("1.3.0", is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("1:2:3:4:5:6:7:8", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:10.0.0.1", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:1.2.3.4", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:0.0.0.0", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("1:2:3:4:5:6:77:88", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:255.255.255.255", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("fe08::7:8", is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("", is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("1:2", is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("fe08", is_ip) == false);
}
