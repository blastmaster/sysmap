#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"

#include "extractors/lustre_extractor.hpp"
#include "utils.hpp"

namespace sysmap { namespace extractor {

    Lustre_Extractor::Lustre_Extractor() : Extractor("Lustre") {}

    void Lustre_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        //TODO: add collected data to extractor_set

        auto defaults = make_value<Map_value>();
        defaults->add("lmm_stripe_size", make_value<String_value>(std::move(data.defaults.lmm_stripe_size)));
        defaults->add("lmm_stripe_count", make_value<String_value>(std::move(data.defaults.lmm_stripe_count)));
        defaults->add("lmm_stripe_offset", make_value<String_value>(std::move(data.defaults.lmm_stripe_offset)));
        defaults->add("lmm_pattern", make_value<String_value>(std::move(data.defaults.lmm_pattern)));
        defaults->add("lmm_magic", make_value<String_value>(std::move(data.defaults.lmm_magic)));
        defaults->add("lmm_layout_gen", make_value<String_value>(std::move(data.defaults.lmm_layout_gen)));
        findings.add_info("default_values", std::move(defaults));

        if (!data.mountpoints.empty()) {
            auto mountpoints_ = make_value<Map_value>();
            for (const auto& mountpoint : data.mountpoints) {
                auto value = make_value<Map_value>();

                /* value->add("path", make_value<String_value>(std::move(mountpoint.path))); */
                value->add("name", make_value<String_value>(std::move(mountpoint.name)));

                auto mdts_ = make_value<Array_value>();
                for(const auto& mdt1 : mountpoint.mdts) {
                    auto mdt_value = make_value<Map_value>();
                    mdt_value->add("blocks", make_value<String_value>(std::move(mdt1.blocks)));
                    mdt_value->add("used", make_value<String_value>(std::move(mdt1.used)));
                    mdt_value->add("avail", make_value<String_value>(std::move(mdt1.avail)));
                    mdt_value->add("usage_percent", make_value<String_value>(std::move(mdt1.usage_percent)));
                    /* mdt_value->add("path", make_value<String_value>(std::move(mdt1.path))); */
                    mdt_value->add("uuid", make_value<String_value>(std::move(mdt1.uuid)));
                    /* mdt_value->add("index", make_value<Uint_value>(std::move(mdt1.index))); */
                    mdts_->add(std::move(mdt_value));
                }
                value->add("mdts", std::move(mdts_));

                auto osts_ = make_value<Array_value>();
                for(const auto& ost1 : mountpoint.osts) {
                    auto ost_value = make_value<Map_value>();
                    ost_value->add("blocks", make_value<String_value>(std::move(ost1.blocks)));
                    ost_value->add("used", make_value<String_value>(std::move(ost1.used)));
                    ost_value->add("avail", make_value<String_value>(std::move(ost1.avail)));
                    ost_value->add("usage_percent", make_value<String_value>(std::move(ost1.usage_percent)));
                    /* ost_value->add("path", make_value<String_value>(std::move(ost1.path))); */
                    ost_value->add("uuid", make_value<String_value>(std::move(ost1.uuid)));
                    /* ost_value->add("index", make_value<Uint_value>(std::move(ost1.index))); */
                    osts_->add(std::move(ost_value));
                }
                value->add("osts", std::move(osts_));

                mountpoints_->add(std::move(mountpoint.path), std::move(value));
                /* value->add("path", make_value<String_value>(std::move(mountpoint.path))); */

            }
            findings.add_info("mountpoints", std::move(mountpoints_));
        }

        /* if (!data.disks.empty()) { */
        /*     auto disks = make_value<Array_value>(); */
        /*     for (const auto& disk : data.disks) { */
        /*         auto value = make_value<Map_value>(); */

        /*         value->add("name", make_value<String_value>(std::move(disk.name))); */
        /*         value->add("model", make_value<String_value>(std::move(disk.model))); */
        /*         value->add("vendor", make_value<String_value>(std::move(disk.vendor))); */
        /*         value->add("size", make_value<Uint_value>(disk.size)); */
        /*         value->add("device_number", make_value<String_value>(std::move(disk.device_number))); */

        /*         disks->add(std::move(value)); */
        /*     } */
        /*     findings.add_info("disks", std::move(disks)); */
        /* } */
    }

    void Lustre_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }

}} /* closing namespace sysmap::extractor */
