#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "extractor.hpp"
#include "extractor_set.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"

using namespace std;
using namespace sysmap;

TEST_CASE("try extractor registry - add filesystem by tag", "[Extractor]") {
    Extractor_Set extr_set;

    extr_set.add_extractor("Filesystem", std::make_shared<linux::Filesystem_Extractor>());
    REQUIRE(extr_set.empty() == false);
}

TEST_CASE("try extractor registry - instantiate through static method", "[Extractor]") {

    auto le = Extractor::instantiate("filesystem");
    REQUIRE(le != nullptr);
}

TEST_CASE("instantiate extractors by tag - and extract", "[Extractor_Set]") {
    Extractor_Set extr_set;

    extr_set.add_by_tag("filesystem");
    extr_set.add_by_tag("disk");
    REQUIRE(extr_set.empty() == false);
    extr_set.extract();
    extr_set.write(cout, Output_format::JSON);
}

TEST_CASE("list all available extractors by name", "[Extractor]") {
    Extractor_Set extr_set;

    for (const auto& kvp : Extractor::registry()) {
        std::cout << "name: " << kvp.first << "\n";
    }
    REQUIRE(4 == Extractor::registry().size());
}
