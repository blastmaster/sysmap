#ifndef __SYSMAP_LSCPU_EXTRACTOR_HPP__
#define __SYSMAP_LSCPU_EXTRACTOR_HPP__

#include "../extractors/cpuinfo_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Lscpu_Extractor : extractor::Cpuinfo_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Lscpu_Extractor>(); }

        protected:
        virtual data collect() override;

        private:
        static Registrar registrar;
    };

}} // sysmap::linux

#endif /* __SYSMAP_CPUINFO_EXTRACTOR_HPP__ */
