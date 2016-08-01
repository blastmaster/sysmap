#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"

#include "extractors/disk_extractor.hpp"

namespace adafs { namespace extractor {

    Disk_Extractor::Disk_Extractor() : Extractor("Disk") {}

    void Disk_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        if (!data.disks.empty()) {
            auto disks = make_value<Map_value>();
            for (const auto& disk : data.disks) {
                auto value = make_value<Map_value>();

                value->add("name", make_value<String_value>(disk.name));
                value->add("model", make_value<String_value>(std::move(disk.model)));
                value->add("vendor", make_value<String_value>(std::move(disk.vendor)));
                value->add("size", make_value<Uint_value>(disk.size));
                value->add("device_number", make_value<String_value>(std::move(disk.device_number)));

                disks->add(std::move(disk.name), std::move(value));
            }
            findings.add_info("disks", std::move(disks));
        }
    }

}} /* closing namespace adafs::extractor */
