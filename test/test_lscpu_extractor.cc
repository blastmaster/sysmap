#include <cstdint>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "extractor.hpp"
#include "extractor_set.hpp"
#include "extractors/cpuinfo_extractor.hpp"
#include "linux/lscpu_extractor.hpp"

#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"

using namespace std;
using namespace sysmap;

TEST_CASE("try extractor registry - add cpuinfo by tag", "[Extractor]")
{
    Extractor_Set extractor_set;
    extractor_set.add_extractor("lscpu", std::make_shared<linux::Lscpu_Extractor>());
    REQUIRE(extractor_set.empty() == false);

    extractor_set.extract();

    using Uint_value = Scalar_value<uint64_t>;

    auto uint_val = extractor_set.get<Uint_value>("SocketCount");
    REQUIRE(uint_val != nullptr);

    uint_val = extractor_set.get<Uint_value>("CorePerSocket");
    REQUIRE(uint_val != nullptr);

    uint_val = extractor_set.get<Uint_value>("CoreCount");
    REQUIRE(uint_val != nullptr);

    uint_val = extractor_set.get<Uint_value>("ThreadPerCore");
    REQUIRE(uint_val != nullptr);

    using Double_value = Scalar_value<double>;
    auto double_val = extractor_set.get<Double_value>("CpuFreq");
    REQUIRE(double_val != nullptr);

    double_val = extractor_set.get<Double_value>("CpuFreqMax");
    REQUIRE(double_val != nullptr);

    double_val = extractor_set.get<Double_value>("CpuFreqMin");
    REQUIRE(double_val != nullptr);

    using String_value = Scalar_value<std::string>;
    auto str_val = extractor_set.get<String_value>("ModelName");
    REQUIRE(str_val != nullptr);

    auto flags = extractor_set.get<Array_value>("CpuFlags");
    REQUIRE(flags != nullptr);
    REQUIRE(flags->empty() == false);

    auto cache = extractor_set.get<Map_value>("L1");
    REQUIRE(cache != nullptr);
    REQUIRE(cache->size() == 2);
    REQUIRE(cache->get<Uint_value>("Size") != nullptr);
    REQUIRE(cache->get<String_value>("Unit") != nullptr);

    cache = extractor_set.get<Map_value>("L2");
    REQUIRE(cache != nullptr);
    REQUIRE(cache->size() == 2);
    REQUIRE(cache->get<Uint_value>("Size") != nullptr);
    REQUIRE(cache->get<String_value>("Unit") != nullptr);

    cache = extractor_set.get<Map_value>("L3");
    REQUIRE(cache != nullptr);
    REQUIRE(cache->size() == 2);
    REQUIRE(cache->get<Uint_value>("Size") != nullptr);
    REQUIRE(cache->get<String_value>("Unit") != nullptr);
}
