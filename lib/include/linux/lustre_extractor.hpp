#ifndef __SYSMAP_LUSTRE_EXTRACTOR_HPP__
#define __SYSMAP_LUSTRE_EXTRACTOR_HPP__

#include "../extractors/disk_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Lustre_Extractor : extractor::Lustre_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Lustre_Extractor>(); }

        protected:
            virtual data collect() override;

        private:
            static Registrar registrar;
            /* void collect_disk_data(data& result); */
    };

}} /* closing namespace sysmap::linux */

#endif /* __SYSMAP_LUSTRE_EXTRACTOR_HPP__ */
