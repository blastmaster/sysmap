#include "linux/get_machine_info_extractor.hpp"
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
 * */

namespace adafs { namespace linux {

    Registrar Get_Machine_Info_Extractor::registrar("get_hostname", &Get_Machine_Info_Extractor::create);

    Get_Machine_Info_Extractor::data Get_Machine_Info_Extractor::collect()
    {
        data result;
        collect_machine_hostname(result);
        collect_kernel_version(result);
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
        result.machine_info.hostname = sys_info.nodename;
    }

    void Get_Machine_Info_Extractor::collect_kernel_version(data& result)
    {
//        std::ifstream file("/proc/version");
//        std::getline(file, result.machine_info.kernel_version);
//        file.close();
       utsname sys_info;
       uname(&sys_info);
       result.machine_info.kernel_version = sys_info.release;
    }

    void Get_Machine_Info_Extractor::collect_kernel_modules(data& result)
    {
        std::ifstream file("/proc/modules");
        std::string str, key;
        int position;
        while(std::getline(file, str))
        {
            position = str.find(" ");
            key = str.substr(0, position);
            if(position != std::string::npos)
            {
                result.machine_info.modules[key] = str.substr(position + 1);
            }
        }
        file.close();
    }
    
}} /* closing namespace adafs::linux */
