#ifndef __SYSMAP_LINUX_NETWORK_DEVICE_EXTRACTOR_HPP__
#define __SYSMAP_LINUX_NETWORK_DEVICE_EXTRACTOR_HPP__

#include "../extractors/network_device_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

struct Network_Device_Extractor : extractor::Network_Device_Extractor
{
    static std::unique_ptr<Extractor> create() { return std::make_unique<Network_Device_Extractor>(); }

    Network_Device_Extractor() {}
    virtual ~Network_Device_Extractor() {}

    protected:
        /*
         * Calls methods to collect information about the Kernel
         * Overrides virtual function from the base class
         */
        virtual data collect() override;

    private:
        static Registrar registrar;

        /*
         * Helper function for collect_device_information(..)
         * It looks up /sys/class/net/${device_name}/type to
         * determine its type since ifaddrs isnt capable of doing that
         */
        std::string get_device_type(const std::string& device_name);

        void collect_device_information(data& result);

};

} }

#endif /* __SYSMAP_LINUX_NETWORK_DEVICE_EXTRACTOR_HPP__ */
