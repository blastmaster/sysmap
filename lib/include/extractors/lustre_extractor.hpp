#ifndef __SYSMAP_EXTRACTOR_LUSTRE_EXTRACTOR_HPP__
#define __SYSMAP_EXTRACTOR_LUSTRE_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

#include <vector>

namespace sysmap { namespace extractor {

    struct Lustre_Extractor : Extractor {

        Lustre_Extractor();

        virtual void load(Extractor_Set& findings) override;
        virtual void store(Extractor_Set& findings, const std::string& dbname) override;

        protected:

        struct abstract_mount
        {
            unsigned blocks;
            unsigned used;
            //available
            unsigned avail;
            unsigned usage_percent;
            std::string path;
        };

        struct mdt : abstract_mount {
            std::string uuid;
        };

        struct ost : abstract_mount {
            std::string uuid;
        };

        struct mountpoint : abstract_mount {
            std::vector<mdt> mdts;
            std::vector<ost> osts;
        }

        struct data
        {
            std::vector<mountpoint> mountpoints;
        };

        virtual data collect() = 0;

    };

}} /* closing namespace sysmap::extractor */

#endif /* __SYSMAP_EXTRACTOR_LUSTRE_EXTRACTOR_HPP__ */
