#include "linux/kernel_extractor.hpp"
#include <string>
#include <vector>
#include <sys/utsname.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

#undef linux

namespace adafs { namespace linux {

    Registrar Kernel_Extractor::registrar("kernel_extractor", &Kernel_Extractor::create);

    Kernel_Extractor::data Kernel_Extractor::collect()
    {
        data result;
        collect_uname(result);
        collect_kernel_config(result);
        collect_kernel_modules(result);

        return result;
    }

    int Kernel_Extractor::get_kernel_config(data& result)
    {
        boost::filesystem::path p("/boot/");
        boost::filesystem::directory_iterator it{p};
        while (it !=  boost::filesystem::directory_iterator{}){
            if(boost::filesystem::is_regular_file(*it) && boost::starts_with(it->path().string(), "/boot/config")){
                std::string filename = std::string("/boot/config-") + std::string(result.system_info.release);
                if(it->path().string().compare(filename) == 0){
                    system(("cat " + std::string(it->path().string()) + " >> /tmp/kernel_config").c_str());
                    std::cout << it->path() << std::endl;
                    return 1;
                }
            }
            it++;
        }

        boost::filesystem::path c("/proc/");
        boost::filesystem::directory_iterator iter{c};
        while (iter !=  boost::filesystem::directory_iterator{}){
            if(boost::filesystem::is_regular_file(*iter) && boost::starts_with(iter->path().string(), "/proc/config")){
                std::string filename = std::string("/proc/config") + std::string(".gz");
                if(iter->path().string().compare(filename) == 0){
                    system(("zcat " + std::string(iter->path().string()) + " >> /tmp/kernel_config").c_str());
                    std::cout << iter->path() << std::endl;
                    return 1;
                }
            }
            iter++;
        }
        return 0;
    }

    void Kernel_Extractor::collect_kernel_config(data& result)
    {
        if(!get_kernel_config(result)){
            adafs::utils::log::logging::debug() << "Couldnt find Kernel Config";
        } else {
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
//                    std::cout << kernel_config.name << " - " << kernel_config.value << std::endl;
                 }
            }
            return true; 
        });
    }
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
