
#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"

#include "extractors/filesystem_extractor.hpp"

namespace adafs { namespace extractor {

    Filesystem_Extractor::Filesystem_Extractor() : Extractor("Filesystem") {}

    void Filesystem_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        if (!data.mountpoints.empty()) {
            auto mounts = make_value<Map_value>();
            for (const auto& mountpoint : data.mountpoints) {

                auto value = make_value<Map_value>();

                value->add("device", make_value<String_value>(mountpoint.device)); // redundant???
                value->add("mount", make_value<String_value>(std::move(mountpoint.mount)));
                value->add("filesystem", make_value<String_value>(std::move(mountpoint.filesystem)));

                // make necessary conversions here, e.g. byte -> mb usw ...
                value->add("size", make_value<Uint_value>(mountpoint.size));
                value->add("available", make_value<Uint_value>(mountpoint.available));

                if (!mountpoint.options.empty()) {
                    auto mount_opts = make_value<Array_value>();
                    for (const auto& option : mountpoint.options) {
                        mount_opts->add(make_value<String_value>(std::move(option)));
                    }
                    value->add("options", std::move(mount_opts));
                }
                mounts->add(std::move(mountpoint.device), std::move(value));
            }
            findings.add_info("mountpoint", std::move(mounts));
        }

        if (!data.partitions.empty()) {
            auto parts = make_value<Map_value>();
            for (const auto& partition : data.partitions) {

                auto value = make_value<Map_value>();

                value->add("name", make_value<String_value>(partition.name));
                value->add("uuid", make_value<String_value>(std::move(partition.uuid)));
                value->add("partuuid", make_value<String_value>(std::move(partition.partition_uuid)));

                if (!partition.label.empty()) {
                    value->add("label", make_value<String_value>(std::move(partition.label)));
                }

                if (!partition.mountpoint.empty()) {
                    value->add("mountpoint", make_value<String_value>(std::move(partition.mountpoint)));
                }

                value->add("size", make_value<Int_value>(partition.size));
                parts->add(std::move(partition.name), std::move(value));
            }
            findings.add_info("partitions", std::move(parts));
        }
    }

}} /* closing namespace adafs::extractor */
