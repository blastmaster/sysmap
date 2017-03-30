#ifndef __ADAFS_EXTRACTOR_KERNEL_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_KERNEL_EXTRACTOR_HPP__ 

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <string>
#include <sys/utsname.h>
#include <vector>
#include <map>
#include <boost/variant.hpp>


namespace adafs { namespace extractor {
    
    struct Kernel_Extractor : Extractor
    {
        Kernel_Extractor() : Extractor("Kernel_Extractor") {}

        virtual ~Kernel_Extractor() {}

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct Kernel_Config
        {
            //Config Parameter
            std::string name;
            //Status 
            boost::variant< std::string, int, bool, char> value;
        };

        struct Modules
        {
            //Name of Module
            std::string name;
            //Size of Module
            int size;
            //State of Module
            bool loaded;
        };

        struct data
        {
            std::vector<Kernel_Config> kernel_config;

            std::vector<Modules> modules;

            utsname system_info;
        };

        virtual data collect() = 0;
    };

}}

#endif /* ifndef __ADAFS_EXTRACTOR_KERNEL_EXTRACTOR_HPP__ */
