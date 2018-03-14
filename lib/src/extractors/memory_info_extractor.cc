#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/memory_info_extractor.hpp"

#include <cstdio>

namespace sysmap { namespace extractor {
    
    void Memory_Info_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();
        
        auto memory = make_value<Map_value>();

        memory->add("TotalMemory", make_value<Uint_value>(data.memory_info.total_memory));
        memory->add("LocalMemory", make_value<Uint_value>(data.memory_info.local_memory));

        auto pages = make_value<Array_value>();
        for (const auto& page : data.memory_info.page_types) {
            auto value = make_value<Map_value>();

            value->add("PageSize", make_value<Uint_value>(page.size));
            value->add("Count", make_value<Uint_value>(page.count));

            pages->add(std::move(value));
        }
        memory->add("PageTypes", std::move(pages));
        findings.add_info("memoryinfo", std::move(memory));
    }

    void Memory_Info_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }

    Memory_Info_Extractor::Memory_Info_Extractor() : Extractor("Memory_Extractor")
    {
    }

    Memory_Info_Extractor::~Memory_Info_Extractor()
    {
    }

}} /* closing namesapce sysmap::extractor */
