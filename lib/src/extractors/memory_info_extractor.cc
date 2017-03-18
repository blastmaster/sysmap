#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/memory_info_extractor.hpp"

#include <sqlite_modern_cpp.h>

#include <cstdio>

using namespace sqlite;

namespace adafs { namespace extractor {
    
    static void create_memory_info_table(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS memory_info (\
                total_memory integer,\
                local_memory integer\
            );";
    }
    
    static void insert_memory_info(const uint64_t tmem, const uint64_t lmem, database& db)
    {
        db << "INSERT INTO memory_info VALUES (?, ?);"
            << (int) tmem
            << (int) lmem;
    }
    
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
        database db(dbname);
        adafs::utils::log::logging::debug() << "hwloc extractor created database object: " << dbname;
        create_memory_info_table(db);
        adafs::utils::log::logging::debug() << "hwloc extractor created memory_info table";
        
        auto mem_info = findings.get<Map_value>("MemoryInfo");
        uint64_t totalmem = mem_info->get<Uint_value>("TotalMemory")->value();
        uint64_t localmem = mem_info->get<Uint_value>("LocalMemory")->value();

        insert_memory_info(totalmem, localmem, db);
        adafs::utils::log::logging::debug() << "hwloc extractor inserted memory info";
    }

    Memory_Info_Extractor::Memory_Info_Extractor() : Extractor("Memory_Extractor")
    {
    }

    Memory_Info_Extractor::~Memory_Info_Extractor()
    {
    }

}} /* closing namesapce adafs::extractor */

