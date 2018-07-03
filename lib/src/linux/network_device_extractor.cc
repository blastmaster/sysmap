#include "linux/network_device_extractor.hpp"

#include <string>
//TODO: check which includes are rly needed!
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

#undef linux

namespace sysmap { namespace linux {

    Registrar Network_Device_Extractor::registrar("network_device_extractor", &Network_Device_Extractor::create);

    Network_Device_Extractor::data Network_Device_Extractor::collect(){
        data result;
        collect_device_information(result);

        return result;
    }

    std::string Network_Device_Extractor::get_device_type(const std::string& device_name){
        std::stringstream filepath;
        filepath << "/sys/class/net/" << device_name << "/type";
        std::ifstream file(filepath.str(), std::ios::in);

        if(!file.is_open()){
            utils::log::logging::debug() << "[sysmap::linux::network_device_extractor]"
                                         << " Unable to open " << filepath.str();
            return "Unknown";
        } else {
            std::string line;
            getline(file,line);
            file.close();
            return line;
        }

    }

    void Network_Device_Extractor::collect_device_information(data& result){
        //mapping device-name to a vector of ip_addr
        std::map<std::string, std::vector<std::string>> network_devices;

        struct ifaddrs *ifaddr, *ifa;
        int family, n, s;
        char host[NI_MAXHOST];

        if (getifaddrs(&ifaddr) == -1) {
            utils::log::logging::error() << "[sysmap::linux::network_device_extractor]"
                                         <<" Calling getifaddrs(..) failed!";
            //TODO: return? --> collect() would proceed with empty data
            return;
        }

        for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
            if (ifa->ifa_addr == NULL)
                continue;

            family = ifa->ifa_addr->sa_family;

            //getType by Name
            auto type = get_device_type( ifa->ifa_name);
            auto name =  ifa->ifa_name;

            Network_Device device(name, type);

            /* For an AF_INET* interface address, display the address */

            if (family == AF_INET || family == AF_INET6) {
                s = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                              sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST,
                        NULL, 0, NI_NUMERICHOST);
                if (s != 0) {
                    printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                }
                network_devices[name].push_back(host);
            }
        }

        for(const auto& network_device : network_devices){
            auto type = get_device_type(network_device.first);
            Network_Device device(network_device.first, type);
            for(const auto& ip : network_device.second){
                device.ip_addr.push_back(ip);
            }
            result.network_devices.push_back(device);
        }
    }

}}
