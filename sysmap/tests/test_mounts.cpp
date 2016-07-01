#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "mounts.hpp"

using namespace adafs;


TEST_CASE("load mounts with getmntent from proc/mounts", "[mounts]") {
    std::string filename {"../test_data/mounts/prometheusprocmounts.txt"};
    auto mvec = load_mount_with_getmntent(filename);

    REQUIRE(mvec.size() == 29);

    SECTION("test first mount entry in vector") {
        auto entry = mvec[0];

        REQUIRE(entry.device_name == "sysfs");
        REQUIRE(entry.mountpoint == "/sys");
        REQUIRE(entry.fs_type == "sysfs");

        std::vector<std::string> opts {"rw", "nosuid", "nodev", "noexec", "relatime"};
        REQUIRE(entry.mount_opts == opts);

        REQUIRE(entry.dump_freq == 0);
        REQUIRE(entry.pass_number == 0);
    }

    SECTION("test last mount entry in vector") {
        auto entry = mvec[mvec.size() - 1];

        REQUIRE(entry.device_name == "taurus:/home/soeste/");
        REQUIRE(entry.mountpoint == "/home/soeste/taurus");
        REQUIRE(entry.fs_type == "fuse.sshfs");

        std::vector<std::string> mopts {"rw", "nosuid", "nodev", "relatime", "user_id=1000", "group_id=1000"};
        REQUIRE(entry.mount_opts == mopts);
        REQUIRE(entry.dump_freq == 0);
        REQUIRE(entry.pass_number == 0);
    }

}
