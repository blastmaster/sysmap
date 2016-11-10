#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"

#include "extractors/disk_extractor.hpp"
#include "utils.hpp"

#include <sqlite_modern_cpp.h>

using namespace sqlite;

namespace adafs { namespace extractor {

    static void create_disk_table(database &db)
    {
        db << "CREATE TABLE IF NOT EXISTS disks (\
                    name string,\
                    model string,\
                    vendor string,\
                    size int, \
                    device_number string not null primary key\
                );";
    }


    static void insert_disk(const std::string& name,
                            const std::string& model,
                            const std::string& vendor,
                            const uint64_t size,
                            const std::string& device_number, database &db)
    {
        db << "INSERT INTO disks VALUES (?, ?, ?, ?, ?);"
            << name
            << model
            << vendor
            << (int) size
            << device_number;
    }


    static int insert_disks(const Array_value* disks, database &db)
    {
        int inserted = 0;
        for (const auto& disk : *disks) {
            auto disk_map = disk.get()->as<Map_value>();
            std::string name = disk_map->get<String_value>("name")->value();
            std::string model = disk_map->get<String_value>("model")->value();
            std::string vendor = disk_map->get<String_value>("vendor")->value();
            uint64_t size = disk_map->get<Uint_value>("size")->value();
            std::string device_number = disk_map->get<String_value>("device_number")->value();
            insert_disk(name, model, vendor, size, device_number, db);
            ++inserted;
        }

        return inserted;
    }


    Disk_Extractor::Disk_Extractor() : Extractor("Disk") {}

    void Disk_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        if (!data.disks.empty()) {
            auto disks = make_value<Array_value>();
            for (const auto& disk : data.disks) {
                auto value = make_value<Map_value>();

                value->add("name", make_value<String_value>(std::move(disk.name)));
                value->add("model", make_value<String_value>(std::move(disk.model)));
                value->add("vendor", make_value<String_value>(std::move(disk.vendor)));
                value->add("size", make_value<Uint_value>(disk.size));
                value->add("device_number", make_value<String_value>(std::move(disk.device_number)));

                disks->add(std::move(value));
            }
            findings.add_info("disks", std::move(disks));
        }
    }

    void Disk_Extractor::store(Extractor_Set& findings)
    {
        // TODO: should database name come from parameter
        std::string dbname = "host.db";
        database db(dbname);
        adafs::utils::log::logging::debug() << "disk extractor created database object: " << dbname;
        create_disk_table(db);
        adafs::utils::log::logging::debug() << "created disk table";

        // insert disks
        auto disks = findings.get<Array_value>("disks");
        int inserted_disks = insert_disks(disks, db);
        adafs::utils::log::logging::debug() << "inserted disks: " << inserted_disks;

    }

}} /* closing namespace adafs::extractor */
