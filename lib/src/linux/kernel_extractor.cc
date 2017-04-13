#include "linux/kernel_extractor.hpp"
#include <string>
#include <vector>
#include <sys/utsname.h>
#include <boost/algorithm/string/predicate.hpp>

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
            std::string str(line);

            if(!boost::starts_with(str, "#") && !str.empty())
            {
                auto position = str.find("=");
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
           std::string str(line);
           auto position = str.find(" ");
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
           }
           return true;
        });
    }

    void Kernel_Extractor::collect_uname(data& result)
    {
        if (uname(&result.system_info) != 0){
            adafs::utils::log::logging::error() << "Calling uname(&result.system_info) failed";
        } else {
            adafs::utils::log::logging::debug() << "Called uname(&result.system_info) successfully";
        }
    }

}}
