#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils.hpp"
#include <arpa/inet.h>

TEST_CASE("Test if IPv4/IPv6 Addresses are recognized"){
    REQUIRE(sysmap::utils::file::for_each_substring("10.0.0.1", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("192.68.2.4", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("1.1.1.1:/mnt/foo", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("8.8.8.8", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("255.255.255.0", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("234.245.231.2", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("ThisStringContainsFoo234.245.231.2MoreFoo", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("ThisStringContainsFoo234.265.231.2MoreFoo", sysmap::utils::network::is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("6.266.1.1", sysmap::utils::network::is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("1.3.0", sysmap::utils::network::is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("1:2:3:4:5:6:7:8", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:10.0.0.1", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:1.2.3.4", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:0.0.0.0", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("1:2:3:4:5:6:77:88", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("::ffff:255.255.255.255", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("fe08::7:8", sysmap::utils::network::is_ip) == true);
    REQUIRE(sysmap::utils::file::for_each_substring("", sysmap::utils::network::is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("1:2", sysmap::utils::network::is_ip) == false);
    REQUIRE(sysmap::utils::file::for_each_substring("fe08", sysmap::utils::network::is_ip) == false);
}
