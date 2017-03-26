#ifndef __ADAFS_HWLOC_MACHINE_INFO_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_MACHINE_INFO_EXTRACTOR_HPP__

#include "../extractors/hwloc_extractor.hpp"

extern "C"{
#include <hwloc.h>
}


namespace adafs { namespace hwloc {

    struct Hwloc_Machine_Info_Extractor : extractor::Hwloc_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_Machine_Info_Extractor>(); }

        Hwloc_Machine_Info_Extractor();

        virtual ~Hwloc_Machine_Info_Extractor();

        protected:
            virtual data collect() override;
            hwloc_topology_t topology;

        private:
            static Registrar registrar;
            void collect_machine_info(data& result);

    };

}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_MACHINE_INFO_EXTRACTOR_HPP__ */
