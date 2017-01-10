#ifndef __ADAFS_DISK_EXTRACTOR_HPP__
#define __ADAFS_DISK_EXTRACTOR_HPP__

#include "../extractors/disk_extractor.hpp"

#undef linux

namespace adafs { namespace linux {

    struct Disk_Extractor : extractor::Disk_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Disk_Extractor>(); }

        protected:
            virtual data collect() override;

        private:
            static Registrar registrar;
            void collect_disk_data(data& result);
    };

}} /* closing namespace adafs::linux */

#endif /* __ADAFS_DISK_EXTRACTOR_HPP__ */
