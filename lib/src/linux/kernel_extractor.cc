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
        for (auto p : paths) 
        {
            boost::filesystem::directory_iterator it{p}; //create directory_iterator
            std::string filename = (p.string() + "config").c_str(); //create filename including its path

            while (it !=  boost::filesystem::directory_iterator{}) //iterate through every file in directory
            {
                std::string path_string(it->path().string()); 
                if(boost::filesystem::is_regular_file(*it) && boost::starts_with(path_string, filename)) 
                {
                    std::string uname_filename = std::string("config-") + std::string(result.system_info.release); //uname_filname is same as config-$(uname -r)
                    std::vector<std::string> filenames {uname_filename, "config.gz", (uname_filename + ".gz").c_str()}; 

                    for (auto filename_ : filenames)
                    {
                        if(path_string.compare((p.string() + filename_)) == 0) //if p.string() is same as filename_ 
                        {
                            if(boost::ends_with(path_string, ".gz"))
                            {
                                system(("zcat " + std::string(path_string) + " >> /tmp/kernel_config").c_str());
                                return 1;
                            } 
                            system(("cat " + std::string(path_string) + " >> /tmp/kernel_config").c_str());
                            return 1;
                        }
                    }
                }
                it++;
            }
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
