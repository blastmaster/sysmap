#ifndef __ADAFS_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <vector>
#include <map>


namespace adafs { namespace extractor {

    /**
     * @class Machine_Info_Extractor
     * Machine_info_Extractor type abstract base class
     */

    struct Machine_Info_Extractor : Extractor
    {
        /*
         * Constructor
         */
        Machine_Info_Extractor() : Extractor("Machine_Info") {}

        /*
         * Virtual desructor
         */
        virtual ~Machine_Info_Extractor() {}

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct Machine_Info
        {
            //sysname - name of this implemantation of the operating system
            std::string sysname;
            // nodename
            std::string nodename;
            //release
            std::string release;
            //machine - name of hardware type on which system is running.
            std::string machine;
            // Kernel Version
            std::string version;
            // Loaded Kernel Modules - <module name, [size, used (0 or 1)]>
            std::map<std::string, std::array<int, 2>> modules;
        };

        struct data
        {
            Machine_Info machine_info;
        };
        
        virtual data collect() = 0;
    };

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__ */
