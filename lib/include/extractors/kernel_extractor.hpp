#ifndef __SYSMAP_EXTRACTOR_KERNEL_EXTRACTOR_HPP__
#define __SYSMAP_EXTRACTOR_KERNEL_EXTRACTOR_HPP__ 

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/filesystem.hpp>

extern "C" {
#include <sys/utsname.h>
};


namespace sysmap { namespace extractor {
namespace fs = boost::filesystem;

    /*
     * @class Kernel_Extractor
     * Kernel_Extractor type abstract base class
     */
    
    struct Kernel_Extractor : Extractor
    {

        /*
         * Constructor
         */
        Kernel_Extractor() : Extractor("Kernel_Extractor") {}

        /*
         * Virtual destructor
         */
        virtual ~Kernel_Extractor() {}

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        /*
         * Vector containing Paths where Kernel Config could be located
         */
        std::vector<fs::path> paths {"/boot/", "/proc/"};

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
        
        /*
         * Calls methods to collect information about the Kernel
         * Pure virtual method will be implemented in derived classes
         */
        virtual data collect() = 0;
    };

}}

#endif /* ifndef __SYSMAP_EXTRACTOR_KERNEL_EXTRACTOR_HPP__ */
