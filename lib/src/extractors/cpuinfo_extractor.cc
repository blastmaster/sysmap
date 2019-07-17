#include "value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"

#include "extractors/cpuinfo_extractor.hpp"
#include "utils.hpp"

namespace sysmap { namespace extractor {

Cpuinfo_Extractor::Cpuinfo_Extractor() : Extractor("Cpuinfo") {}

void Cpuinfo_Extractor::load(Extractor_Set& findings)
{
    auto data = collect();

    if (!data.cpuinfo.empty()) {
        auto cpus = make_value<Array_value>();
        // TODO:
        findings.add_info("cpuinfo", std::move(cpus));
    }
}

void Cpuinfo_Extractor::store(Extractor_Set& findings, const std::string& dbname)
{
}

}} // sysmap::extractor
