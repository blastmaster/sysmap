#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/network_device_extractor.hpp"

namespace sysmap { namespace extractor {

    void Network_Device_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        auto devices = make_value<Array_value>();

        for(const auto& device : data.network_devices){
            auto value = make_value<Map_value>();
            value->add("Name", make_value<String_value>(device.name));
            value->add("Type", make_value<String_value>(device.type));
            //TODO: Getting IPs into Array_value or something like that
            /* value->add("Addresses", make_value<Array_value>()) */
        }

        data.network_devices.clear();

        findings.add_info("deviceinfo", std::move(devices));

    }

    void Network_Device_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }

} /* closing namespace extractor */
} /* closing namespace sysmap */
