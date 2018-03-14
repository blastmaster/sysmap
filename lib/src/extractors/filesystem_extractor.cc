#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"
#include "utils.hpp"

#include "extractors/filesystem_extractor.hpp"

namespace sysmap { namespace extractor {

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
                value->add("blksize", make_value<Uint_value>(mountpoint.blksize));
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

    void Filesystem_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }

}} /* closing namespace sysmap::extractor */
