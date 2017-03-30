#include "linux/machine_info_extractor.hpp"
#include <unistd.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sys/utsname.h>

#undef linux

/* 
 * possible methods: 
 *
 *      hostname        : "/etc/hostname", utsname.nodename ("machine's network node hostname"), gethostname()
 *      kernel_version  : "/proc/version", utsname.version
 *      kernel_modules  : "/proc/modues" -> lsmod also just reads /proc/modules
 * 
 * 
 * utsname struct members:
 * char  sysname[]  Name of this implementation of the operating system.
 * char  nodename[] Name of this node within the communications network to which this node is attached, if any.
 * char  release[]  Current release level of this implementation.
 * char  version[]  Current version level of this release.
 * char  machine[]  Name of the hardware type on which the system is running.
 */

namespace adafs { namespace linux {

    Registrar Get_Machine_Info_Extractor::registrar("linux_machine_info", &Get_Machine_Info_Extractor::create);

    Get_Machine_Info_Extractor::data Get_Machine_Info_Extractor::collect()
    {
        data result;
//        collect_machine_hostname(result);
//        collect_kernel_version(result);
        collect_machine_info(result);
        collect_kernel_modules(result);

        return result;
    }

    void Get_Machine_Info_Extractor::collect_machine_hostname(data& result)
    {
//        std::ifstream file("/etc/hostname");
//        std::getline(file, result.machine_info.hostname);
//        file.close();
        utsname sys_info;
        uname(&sys_info);
        result.machine_info.nodename = sys_info.nodename;
    }

    void Get_Machine_Info_Extractor::collect_machine_info(data& result)
    {
        utsname sys_info;
        uname(&sys_info);
        result.machine_info.nodename = sys_info.nodename;
        result.machine_info.sysname = sys_info.sysname;
        result.machine_info.release = sys_info.release;
        result.machine_info.version = sys_info.version;
        result.machine_info.machine = sys_info.machine;        
    }

    void Get_Machine_Info_Extractor::collect_kernel_version(data& result)
    {
//        std::ifstream file("/proc/version");
//        std::getline(file, result.machine_info.kernel_version);
//        file.close();
       utsname sys_info;
       uname(&sys_info);
       result.machine_info.release = sys_info.release;
    }

    void Get_Machine_Info_Extractor::collect_kernel_modules(data& result)
    {
        std::ifstream file("/proc/modules");
        std::string str;
        unsigned int position;
        while(std::getline(file, str) && file.is_open()) 
        {
            position = str.find(" ");
            const std::string key = str.substr(0, position);

            if(position != std::string::npos)
            {
                str = str.substr(position + 1);
                position = str.find(" ");
                const int size = std::stoi(str.substr(0, position));
                if(position != std::string::npos)
                {
                    int info [2];
                    info[0] = size;
                    info[1] = std::stoi(str.substr(position +1));
                    result.machine_info.modules[key] = info;
                }
            }
        }
        file.close();
    }
    
}} /* closing namespace adafs::linux */
