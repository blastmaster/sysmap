#ifndef __SYSMAP_CPUINFO_EXTRACTOR_HPP__
#define __SYSMAP_CPUINFO_EXTRACTOR_HPP__

#include "../extractors/cpuinfo_extractor.hpp"

#undef linux

namespace sysmap { namespace linux {

    struct Cpuinfo_Extractor : extractor::Cpuinfo_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Cpuinfo_Extractor>(); }

        protected:
        virtual data collect() override;

        private:
        static Registrar registrar;
        void collect_cpuinfo(data& result);
    };

}} // sysmap::linux

#endif /* __SYSMAP_CPUINFO_EXTRACTOR_HPP__ */
