#ifndef __ADAFS_HWLOC_MEMORY_INFO_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_MEMORY_INFO_EXTRACTOR_HPP__

#include "../extractors/memory_info_extractor.hpp"

extern "C"{
#include <hwloc.h>
}


namespace adafs { namespace hwloc {
    
    struct Hwloc_Memory_Info_Extractor : extractor::Memory_Info_Extractor
    {

        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_Memory_Info_Extractor>(); }
        
        Hwloc_Memory_Info_Extractor();

        virtual ~Hwloc_Memory_Info_Extractor();

        protected:
            virtual data collect() override;
            hwloc_topology_t topology;

       
        private:
            static Registrar registrar;
            void collect_memory_info(data& result);
    };    


}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_MEMORY_INFO_EXTRACTOR_HPP__ */
