#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "proc_partitions.hpp"

using namespace adafs;


TEST_CASE("load partitions from proc/partitions", "[proc_partitions]") {
    std::string filename {"../test_data/partitions/prometheuspartitions.txt"};
    Proc_Partition::load_partitions(filename);

    REQUIRE(Proc_Partition::device_path_cache.size() == 6);

    SECTION("test first partition entry (sda)") {
        auto entry = Proc_Partition::proc_partition_cache["/dev/sda"];

        REQUIRE(entry.partition_name == "sda");
        REQUIRE(entry.num_blocks == 250059096);
        REQUIRE(entry.major_dev_num == 8);
        REQUIRE(entry.minor_dev_num == 0);
    }

    SECTION("test last partition entry (dm-1)") {
        auto entry = Proc_Partition::proc_partition_cache["/dev/dm-1"];

        REQUIRE(entry.partition_name == "dm-1");
        REQUIRE(entry.num_blocks == 6834176);
        REQUIRE(entry.major_dev_num == 254);
        REQUIRE(entry.minor_dev_num == 1);
    }

    SECTION("test device name") {
        auto entry = Proc_Partition::proc_partition_cache["/dev/dm-1"];
        std::string devname = entry.device_path();
        REQUIRE(devname == "/dev/dm-1");
    }

}
