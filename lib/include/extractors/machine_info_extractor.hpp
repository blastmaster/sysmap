#ifndef __ADAFS_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"
#include <vector>
#include <map>


namespace adafs { namespace extractor {

    struct Machine_Info_Extractor : Extractor
    {
        Machine_Info_Extractor() : Extractor("Machine_Info") {}

        virtual ~Machine_Info_Extractor() {}

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct Machine_Info
        {
            // HostName
            std::string hostname;
            // Kernel Version
            std::string kernel_version;
            // Loaded Kernel Modules
            //TODO: Think about required information, maybe vector<string> filled with module Names is enough?
            std::map<std::string, std::string> modules;
        };

        struct data
        {
            Machine_Info machine_info;
        };

        virtual data collect() = 0;
    };

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_MACHINE_INFO_EXTRACTOR_HPP__ */
