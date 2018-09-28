#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils.hpp"

TEST_CASE("Test if IPv4 Addresses are recognized"){
    REQUIRE(sysmap::utils::parsing::is_remote_address("10.0.0.1") == true);
    REQUIRE(sysmap::utils::parsing::is_remote_address("192.68.2.4") == true);
    REQUIRE(sysmap::utils::parsing::is_remote_address("1.1.1.1") == true);
    REQUIRE(sysmap::utils::parsing::is_remote_address("8.8.8.8") == true);
    REQUIRE(sysmap::utils::parsing::is_remote_address("255.255.255.0") == true);
    REQUIRE(sysmap::utils::parsing::is_remote_address("234.245.231.2") == true);
    REQUIRE(sysmap::utils::parsing::is_remote_address("256.1.1.1") == false);
    REQUIRE(sysmap::utils::parsing::is_remote_address("1.3.0") == false);
    REQUIRE(sysmap::utils::parsing::is_remote_address("300.2.3.4") == false);
}

TEST_CASE("Test if IPv6 Addresses are recognized"){
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6:7:8"));
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("::ffff:10.0.0.1"));
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("::ffff:1.2.3.4"));
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("::ffff:0.0.0.0"));
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6:77:88"));
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("::ffff:255.255.255.255"));
    REQUIRE(true == sysmap::utils::parsing::is_remote_address("fe08::7:8"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address(""));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6:7:8:9"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6::7:8"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address(":1:2:3:4:5:6:7:8"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6:7:8:"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("::1:2:3:4:5:6:7:8"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6:7:8::"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("1:2:3:4:5:6:7:88888"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("2001:db8:3:4:5::192.0.2.33"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("fe08::7:8%"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("fe08::7:8i"));
    REQUIRE(false == sysmap::utils::parsing::is_remote_address("fe08::7:8interface"));
}

TEST_CASE("Test if Hostnames are recognized"){
    REQUIRE(true == sysmap::utils::parsing::is_hostname("www.tu-dresden.de"));
    REQUIRE(true == sysmap::utils::parsing::is_hostname("google.de"));
    REQUIRE(true == sysmap::utils::parsing::is_hostname("gov.ru"));
    REQUIRE(true == sysmap::utils::parsing::is_hostname("data.subdomain.foo.bar"));
    REQUIRE(true == sysmap::utils::parsing::is_hostname("ada-fs.io"));
    REQUIRE(true == sysmap::utils::parsing::is_hostname("google"));
    REQUIRE(false == sysmap::utils::parsing::is_hostname("google."));
    REQUIRE(false == sysmap::utils::parsing::is_hostname("google:das.de"));

}
