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

            struct default_values {
                std::string lmm_stripe_size;
                std::string lmm_stripe_count;
                std::string lmm_stripe_offset;
                std::string lmm_pattern;
                std::string lmm_magic;
                std::string lmm_layout_gen;
            };

            struct abstract_mount
            {
                std::string blocks;
                std::string used;
                std::string avail;
                std::string usage_percent;
                std::string path;
            };

            //TODO: use more generic naming, representing mdt and ost
            struct mdt : abstract_mount {
                std::string uuid;
                std::string index;
            };

            struct mountpoint : abstract_mount {
                std::string name;
                std::vector<mdt> mdts;
                std::vector<mdt> osts;
            };

            struct data
            {
                std::vector<mountpoint> mountpoints;
                default_values defaults;
            };

            virtual data collect() = 0;

    };

}} /* closing namespace sysmap::extractor */

#endif /* __SYSMAP_EXTRACTOR_LUSTRE_EXTRACTOR_HPP__ */
