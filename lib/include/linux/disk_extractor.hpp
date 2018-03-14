#ifndef __SYSMAP_DISK_EXTRACTOR_HPP__
#define __SYSMAP_DISK_EXTRACTOR_HPP__

#include "../extractors/disk_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Disk_Extractor : extractor::Disk_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Disk_Extractor>(); }

        protected:
            virtual data collect() override;

        private:
            static Registrar registrar;
            void collect_disk_data(data& result);
    };

}} /* closing namespace sysmap::linux */

#endif /* __SYSMAP_DISK_EXTRACTOR_HPP__ */
