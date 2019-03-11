#include "linux/network_device_extractor.hpp"

#include <string>

extern "C" {
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
};

#undef linux

namespace sysmap { namespace linux {

    Registrar Network_Device_Extractor::registrar("network_device_extractor",
                                                  &Network_Device_Extractor::create);

    Network_Device_Extractor::data Network_Device_Extractor::collect(){
        data result;
        collect_device_information(result);

        return result;
    }

    std::string Network_Device_Extractor::get_device_type(const std::string& device_name){
        std::string type("UNKNOWN");

        std::stringstream filepath;
        filepath << "/sys/class/net/" << device_name << "/type";
        std::ifstream file(filepath.str(), std::ios::in);

        if(!file.is_open()){
            utils::log::logging::debug() << "[sysmap::linux::network_device_extractor]"
                                         << " Unable to open " << filepath.str();
            return type;
        }

        std::string line;
        getline(file,line);
        file.close();

        //Assing Name of Devicetype defined in <net/if_arp.h>
        //If no match is found, type will remain "UNKNOWN"
        if(line == "1"){
            type = "ETHERNET";
        } else if (line == "32"){
            type = "INFINIBAND";
        } else if (line == "772"){
            type = "LOOPBACK";
        }

        return type;
    }

    void Network_Device_Extractor::clean_up_ip_aliasing(std::string& device_name){
        //Check if the device_name contains a ':'
        const auto find_colon = device_name.find(':');
        if(find_colon == std::string::npos){
            return;
        }
        //If ':' is found, erase everything from ':' to the end of the String
        device_name.erase(find_colon);
    }

    void Network_Device_Extractor::collect_device_information(data& result){
        //mapping device-name to a Network_Device
        std::map<std::string, Network_Device> network_devices;

        struct ifaddrs *ifaddr, *ifa;
        int family;
        char ip_addr[NI_MAXHOST];

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

            //get device name
            std::string name = ifa->ifa_name;

            //WARINING: clean_up_ip_aliasing REMOVES :*. CHANGES ib0:* to ib0
            //If that ib0:0 device should be stored as an extra device
            //this function has to be called in get_device_type(..)
            clean_up_ip_aliasing(name);
            auto type = get_device_type(name);

            //Construct Network_Device in-place because copy assignment is deleted
            network_devices.emplace(std::make_pair(name, Network_Device(name, type)));

            //Collect assigned IPv4 and IPv6 addresses by calling getnameinfo(..)
            if (family == AF_INET || family == AF_INET6) {
                int ret = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                              sizeof(struct sockaddr_in6),
                        ip_addr, NI_MAXHOST,
                        nullptr, 0, NI_NUMERICHOST);
                if (ret != 0) {
                    utils::log::logging::error() << "[sysmap::linux::network_device_extractor]"
                                                 << " getnameinfo() failed: " << gai_strerror(ret)
                                                 << "\n";
                    continue;
                }

                network_devices.at(name).ip_addr.push_back(ip_addr);
            }
        }

        freeifaddrs(ifaddr);

        for(const auto& device : network_devices) {
            result.network_devices.push_back(device.second);
        }
    }
}}
