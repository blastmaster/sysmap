#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"
#include "utils.hpp"

#include "extractors/filesystem_extractor.hpp"

#include <sqlite_modern_cpp.h>

using namespace sqlite;

namespace adafs { namespace extractor {

    static void create_mount_table(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS mountpoints (\
                device string,\
                mountpoint string,\
                filesystem string,\
                size integer,\
                available integer,\
                options string\
            );";
    }

    static void create_partition_table(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS partitions (\
                name string,\
                device_number string,\
                uuid string,\
                partuuid string,\
                label string,\
                mountpoint string,\
                size integer\
            );";
    }

    static void insert_mountpoint(const std::string& device,
                                  const std::string& mntpnt,
                                  const std::string& filesys,
                                  const uint64_t& size,
                                  const uint64_t& avail,
                                  const std::string& options, database& db)
    {
        db << "INSERT INTO mountpoints VALUES (?, ?, ?, ?, ?, ?);"
            << device
            << mntpnt
            << filesys
            << (int) size
            << (int) avail
            << options;
    }

    static void insert_partition(const std::string& name,
                                 const std::string& device_number,
                                 const std::string& uuid,
                                 const std::string& partuuid,
                                 const std::string& label,
                                 const std::string& mntpnt,
                                 const int& size, database& db)
    {
        // TODO: device_number as primary key or uuid?
        db << "INSERT INTO partitions VALUES (?, ?, ?, ?, ?, ?, ?);"
            << name
            << device_number
            << uuid
            << partuuid
            << label
            << mntpnt
            << size;
    }

    static int insert_partitions(const Array_value* partitions, database& db)
    {
        int inserted = 0;
        for (const auto& part : *partitions) {
            auto part_map = part.get()->as<Map_value>();
            std::string name = part_map->get<String_value>("name")->value();
            std::string device_number = part_map->get<String_value>("device_number")->value();
            std::string uuid = part_map->get<String_value>("uuid")->value();
            std::string partuuid {"NONE"};
            if (part_map->get<String_value>("partuuid") != nullptr) {
                partuuid = part_map->get<String_value>("partuuid")->value();
            }
            std::string label {"NONE"};
            if (part_map->get<String_value>("label") != nullptr) {
                label = part_map->get<String_value>("label")->value();
            }
            std::string mntpnt {"NONE"};
            if (part_map->get<String_value>("mountpoint") != nullptr) {
                mntpnt = part_map->get<String_value>("mountpoint")->value();
            }
            int size = part_map->get<Int_value>("size")->value();
            insert_partition(name, device_number, uuid, partuuid, label, mntpnt, size, db);
            ++inserted;
        }

        return inserted;
    }

    static int insert_mountpoints(const Array_value* mounts, database& db)
    {
        int inserted = 0;
        for (const auto& mnt : *mounts) {
            auto mnt_map = mnt.get()->as<Map_value>();
            std::string device = mnt_map->get<String_value>("device")->value();
            std::string mntpnt = mnt_map->get<String_value>("mountpoint")->value();
            std::string filesys = mnt_map->get<String_value>("filesystem")->value();
            uint64_t size = mnt_map->get<Uint_value>("size")->value();
            uint64_t avail = mnt_map->get<Uint_value>("available")->value();
            std::string options = mnt_map->get<String_value>("options")->value();
            insert_mountpoint(device, mntpnt, filesys, size, avail, options, db);
            ++inserted;
        }

        return inserted;
    }

    Filesystem_Extractor::Filesystem_Extractor() : Extractor("Filesystem") {}

    void Filesystem_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        if (!data.mountpoints.empty()) {
            auto mounts = make_value<Array_value>();
            for (const auto& mountpoint : data.mountpoints) {

                auto value = make_value<Map_value>();

                value->add("device", make_value<String_value>(std::move(mountpoint.device))); // redundant???
                value->add("mountpoint", make_value<String_value>(std::move(mountpoint.mount)));
                value->add("filesystem", make_value<String_value>(std::move(mountpoint.filesystem)));

                // make necessary conversions here, e.g. byte -> mb usw ...
                value->add("size", make_value<Uint_value>(mountpoint.size));
                value->add("available", make_value<Uint_value>(mountpoint.available));
                value->add("options", make_value<String_value>(std::move(mountpoint.options)));

                mounts->add(std::move(value));
            }
            findings.add_info("mountpoints", std::move(mounts));
        }

        if (!data.partitions.empty()) {
            auto parts = make_value<Array_value>();
            for (const auto& partition : data.partitions) {

                auto value = make_value<Map_value>();

                value->add("name", make_value<String_value>(std::move(partition.name)));
                value->add("device_number", make_value<String_value>(std::move(partition.device_number)));
                value->add("uuid", make_value<String_value>(std::move(partition.uuid)));

                if (!partition.partition_uuid.empty()) {
                    value->add("partuuid", make_value<String_value>(std::move(partition.partition_uuid)));
                }

                if (!partition.label.empty()) {
                    value->add("label", make_value<String_value>(std::move(partition.label)));
                }

                if (!partition.mountpoint.empty()) {
                    value->add("mountpoint", make_value<String_value>(std::move(partition.mountpoint)));
                }

                value->add("size", make_value<Int_value>(partition.size));
                parts->add(std::move(value));
            }
            findings.add_info("partitions", std::move(parts));
        }
    }

    void Filesystem_Extractor::store(Extractor_Set& findings)
    {
        // TODO: dbname!!!
        std::string dbname = "host.db";
        database db(dbname);
        adafs::utils::log::logging::debug() << "filesystem extractor created database object: " << dbname;
        create_mount_table(db);
        adafs::utils::log::logging::debug() << "filesystem extractor created mount table";
        create_partition_table(db);
        adafs::utils::log::logging::debug() << "filesystem extractor created partition table";

        // insert mounts
        auto mounts = findings.get<Array_value>("mountpoints");
        int inserted_mountpoints = insert_mountpoints(mounts, db);
        adafs::utils::log::logging::debug() << "filesystem extractor inserted mounts: " << inserted_mountpoints;

        // insert partitions
        auto partitions = findings.get<Array_value>("partitions");
        int insterted_partitions = insert_partitions(partitions, db);
        adafs::utils::log::logging::debug() << "filesystem extractor insterted partitions: " << insterted_partitions;

    }

}} /* closing namespace adafs::extractor */
