#ifndef __ADAFS_EXTRACTOR_NETWORK_DEVICE_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_NETWORK_DEVICE_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <string>
#include <vector>

extern "C" {
#include <netinet/in.h>
};

namespace sysmap { namespace extractor {

using typeMap_t = std::map<int, std::string>;

/*
 * @class Network_Device_Extractor
 * Network_Device_Extractor type abstract base class
 */

struct Network_Device_Extractor : Extractor
{
    /*
     * Constructor
     */
    Network_Device_Extractor() : Extractor("Network_Device_Extractor") {}

    /*
     * Virtual destructor
     */
    virtual ~Network_Device_Extractor() {}

    virtual void load(Extractor_Set& findings) override;
    virtual void store(Extractor_Set& findings, const std::string& dbname) override;

    protected:

        struct Network_Device{
            Network_Device(std::string name_, std::string type_) : name(name_), type(type_) {}

            const std::string name, type;
            std::vector<std::string> ip_addr;
        };

        struct data {
            std::vector<Network_Device> network_devices;
        };

        /*
         * Calls methods to collect information about the Network Devices
         * Pure virtual method will be implemented in derived classes
         */
        virtual data collect() = 0;

    private:
        typeMap_t types;
};

} /* closing namespace extractor */ } /*closing namespace sysmap */

#endif /* ifndef __ADAFS_EXTRACTOR_NETWORK_DEVICE_EXTRACTOR_HPP__ */
