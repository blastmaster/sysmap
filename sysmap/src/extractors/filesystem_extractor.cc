
#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"

#include "extractors/filesystem_extractor.hpp"

namespace adafs { namespace extractor {

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
            findings.add("mountpoint", std::move(mounts));
        }

    }

}} /* closing namespace adafs::extractor */
