#include "linux/network_device_extractor.hpp"
#include <string>

#undef linux

namespace sysmap { namespace linux {

    Registrar Network_Device_Extractor::registrar("network_device_extractor", &Network_Device_Extractor::create);

    Network_Device_Extractor::data Network_Device_Extractor::collect(){
        data result;
        collect_device_information(result);

        return result;
    }

    std::string Network_Device_Extractor::get_device_type(const std::string& device_name){

        return " ";
    }

    void Network_Device_Extractor::collect_device_information(data& result){

    }

}}
