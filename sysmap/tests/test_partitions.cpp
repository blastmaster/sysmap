
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "partitions.hpp"


TEST_CASE("load partitions from proc/partitions", "[partitions]") {
    std::string filename {"../test_data/partitions/prometheuspartitions.txt"};
    auto pvec = load_partitions(filename);

    REQUIRE(pvec.size() == 6);

    SECTION("test first partition entry (sda)") {
        auto entry = pvec[0];

        REQUIRE(entry.partition_name == "sda");
        REQUIRE(entry.num_blocks == 250059096);
        REQUIRE(entry.major_dev_num == 8);
        REQUIRE(entry.minor_dev_num == 0);
    }

    SECTION("test last partition entry (dm-1)") {
        auto entry = pvec[pvec.size() - 1];

        REQUIRE(entry.partition_name == "dm-1");
        REQUIRE(entry.num_blocks == 6834176);
        REQUIRE(entry.major_dev_num == 254);
        REQUIRE(entry.minor_dev_num == 1);
    }

}
