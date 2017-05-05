#include "linux/kernel_extractor.hpp"
#include <string>
#include <vector>
#include <sys/utsname.h>
#include <iostream>
#include <fstream>
#include <boost/variant.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

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

    void Kernel_Extractor::collect_kernel_config(data& result)
    {
        //vector<path> paths declared in /include/extractors/kernel_extractor.hpp
        for (auto& p : paths) 
        {

            //create directory_iterator
            boost::filesystem::directory_iterator it{p}; 
            //create searchterm "/path/to/dir/config"
            std::string searchterm = (p.string() + "config").c_str(); 
            
            //iterate through every file in directory p
            while (it !=  boost::filesystem::directory_iterator{}) 
            {
                std::string it_(it->path().string()); 
                //only proceed when it is a regular file and starts with "config"
                if(boost::filesystem::is_regular_file(*it) && boost::starts_with(it_, searchterm)) 
                {
                    //uname_r is same as "config-$(uname -r)"
                    std::string uname_r = std::string("config-") + std::string(result.system_info.release); 
                    std::vector<std::string> filenames {uname_r, "config.gz", (uname_r + ".gz")}; 

                    for (auto& filename_ : filenames)
                    {
                        if(it_.compare((p.string() + filename_)) == 0)
                        {

                            auto stream_iterator = [&](const std::string& line)
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
                            };

                            if(boost::ends_with(it_, ".gz"))
                            {
                                //file needs to be decompressed, using boost gzip_decompressor()
                                std::ifstream file_(it_, std::ios_base::in | std::ios_base::binary);
                                boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
                                in.push(boost::iostreams::gzip_decompressor());
                                in.push(file_);

                                std::istream config_stream( &in );
                                utils::file::for_each_line(config_stream, stream_iterator);
                                return;
                               
                            } 
                            else 
                            {
                                std::ifstream config_stream( it_ );
                                utils::file::for_each_line(config_stream, stream_iterator);
                                return;
                            } 
                        }
                    }
                }
                it++;
            }
        }
        adafs::utils::log::logging::debug() << "[sysmap::linux::kernel_extractor] Could not find Kernel Config file";
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
            adafs::utils::log::logging::error() << "[sysmap::linux::kernel_extractor] Calling uname(&result.system_info) failed";
        }
    }

}}
