#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "extractor_set.hpp"
#include "linux/infiniband_extractor.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "hwloc/hwloc_extractor.hpp"
#include "utils.hpp"


using namespace adafs;


TEST_CASE("try extractor registry", "[Extractor]") {
    Extractor_Set extr_set;

    extr_set.by_tag("filesystem");
    REQUIRE(extr_set.empty() == false);

}
