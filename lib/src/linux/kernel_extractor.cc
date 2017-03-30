#include "linux/kernel_extractor.hpp"
#include <string>
#include <vector>
#include <sys/utsname.h>

#undef linux

namespace adafs { namespace linux {

    Registrar Kernel_Extractor::registrar("kernel_extractor", &Kernel_Extractor::create);

    Kernel_Extractor::data Kernel_Extractor::collect()
    {
        data result;
        collect_kernel_config(result);
        collect_kernel_modules(result);
        collect_uname(result);

        return result;
    }

    void Kernel_Extractor::collect_kernel_config(data& result)
    {
        system("zcat /proc/config.gz >> /tmp/kernel_config");
        utils::file::for_each_line("/tmp/kernel_config", [&](const std::string& line)
        {
            std::string str = line;

            if(str[0] != '#' && str[0] != NULL)
            {
                unsigned int position = str.find("=");
                const std::string name = str.substr(0, position);
                
                if (position != std::string::npos)
                {
                    Kernel_Config kernel_config;
                    kernel_config.name = name;
                    kernel_config.value = str.substr(position + 1);
                    result.kernel_config.push_back(kernel_config);
                 }
            }
            return true; 
        });
    }

    void Kernel_Extractor::collect_kernel_modules(data& result)
    {
        utils::file::for_each_line("/proc/modules", [&](const std::string& line)
        {
           std::string str = line;
           unsigned int position = str.find(" ");
           const std::string name = str.substr(0, position);

           if (position != std::string::npos)
           {
               str = str.substr(position + 1);
               position = str.find(" ");
               const int size = std::stoi(str.substr(0, position));
               Modules module;
               module.name = name;
               module.size = size;
               module.loaded = true;
               result.modules.push_back(module);
               return true;
           }
           return false;
        });
    }

    void Kernel_Extractor::collect_uname(data& result)
    {
        uname(&result.system_info);
    }

}}
