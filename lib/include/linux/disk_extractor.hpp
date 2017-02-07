#ifndef __ADAFS_DISK_EXTRACTOR_HPP__
#define __ADAFS_DISK_EXTRACTOR_HPP__

#include "../extractors/disk_extractor.hpp"

namespace adafs { namespace linux {

    struct Disk_Extractor : extractor::Disk_Extractor {

        protected:

            virtual data collect() override;

        private:

            void collect_disk_data(data& result);

    };

}} /* closing namespace adafs::linux */

#endif /* __ADAFS_DISK_EXTRACTOR_HPP__ */
