#include "linux/network_device_extractor.hpp"

#include <string>

//TODO: check which includes are rly needed!
extern "C" {
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
};

#undef linux

namespace sysmap { namespace linux {

    Registrar Network_Device_Extractor::registrar("network_device_extractor", &Network_Device_Extractor::create);

    Network_Device_Extractor::data Network_Device_Extractor::collect(){
        data result;
        collect_device_information(result);

        return result;
    }

    std::string Network_Device_Extractor::get_device_type(const std::string& device_name){
        //TODO: document that only up interfaces are extracted
        std::stringstream filepath;
        filepath << "/sys/class/net/" << device_name << "/type";
        std::ifstream file(filepath.str(), std::ios::in);

        if(!file.is_open()){
            utils::log::logging::debug() << "[sysmap::linux::network_device_extractor]"
                                         << " Unable to open " << filepath.str();
            return "Unknown";
        }

        std::string line, type;
        getline(file,line);
        file.close();

        //Assing Name of Devicetype defined in <net/if_arp.h>
        if(line == "1"){
            type = "ETHERNET";
        } else if (line == "32"){
            type = "INFINIBAND";
        } else if (line == "772"){
            type = "LOOPBACK";
        } else {
            type = "UNKNOWN";
        }

        return type;
    }

    void Network_Device_Extractor::clean_up_ip_aliasing(std::string& device_name){
        if(device_name.find(":") == std::string::npos){
            return;
        }
        device_name.erase(device_name.find(':'));
    }

    void Network_Device_Extractor::collect_device_information(data& result){
        //mapping device-name to a vector of ip_addr
        std::map<std::string, std::vector<std::string>> network_devices;

        struct ifaddrs *ifaddr, *ifa;
        int family;
        char host[NI_MAXHOST];

        if (getifaddrs(&ifaddr) == -1) {
            //collect() will proceed with empty data
            utils::log::logging::error() << "[sysmap::linux::network_device_extractor]"
                                         << " Calling getifaddrs(..) failed!"
                                         << " No Network Device will be extracted";
            return;
        }

        for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr)
                continue;

            family = ifa->ifa_addr->sa_family;

            //getType by Name
            std::string name = ifa->ifa_name;

            //WARINING: REMOVES :*, CHANGES ib0:* to ib0
            //If that ib0:0 device should be stored as an extra device
            //this function should be called in get_device_type(..)
            clean_up_ip_aliasing(name);

            /* For an AF_INET* interface address, display the address */

            if (family == AF_INET || family == AF_INET6) {
                int ret = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                              sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST,
                        nullptr, 0, NI_NUMERICHOST);
                if (ret != 0) {
                    utils::log::logging::error() << "[sysmap::linux::network_device_extractor]"
                                                 << " getnameinfo() failed: " << gai_strerror(ret)
                                                 << "\n";
                    continue;
                }
                network_devices[name].push_back(host);
            }
        }

        for(auto& network_device : network_devices){
            auto device_name = network_device.first;
            auto type = get_device_type(device_name);
            Network_Device device(device_name, type);
            for(const auto& ip : network_device.second){
                device.ip_addr.push_back(ip);
            }
            result.network_devices.push_back(device);
        }
    }
}}
