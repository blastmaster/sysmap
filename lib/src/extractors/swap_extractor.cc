#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"

#include "extractors/swap_extractor.hpp"

namespace sysmap { namespace extractor {

    Swap_Extractor::Swap_Extractor() : Extractor("Swap") {}

    void Swap_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();
        auto swap = make_value<Map_value>();
        swap->add("filename", make_value<String_value>(std::move(data.info.filename)));
        swap->add("type", make_value<String_value>(std::move(data.info.type)));
        swap->add("size", make_value<Uint_value>(data.info.size));
        swap->add("used", make_value<Uint_value>(data.info.used));
        findings.add_info("swap", std::move(swap));
    }

    void Swap_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }


}} /* closing namespace sysmap::extractor */
