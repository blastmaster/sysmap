#ifndef __SYSMAP_HWLOC_MEMORY_INFO_EXTRACTOR_HPP__
#define __SYSMAP_HWLOC_MEMORY_INFO_EXTRACTOR_HPP__

#include "../extractors/memory_info_extractor.hpp"

extern "C"{
#include <hwloc.h>
}


namespace sysmap { namespace hwloc {

    /**
     *@class Hwloc_Memory_Info_Extractor
     */
    
    struct Hwloc_Memory_Info_Extractor : extractor::Memory_Info_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_Memory_Info_Extractor>(); }
        
        /**
         * Constructor
         * Initializes, sets flags and loads hwloc topology
         */
        Hwloc_Memory_Info_Extractor();

        /**
         * Virtual Destructor
         * Destroys hwloc topology
         */
        virtual ~Hwloc_Memory_Info_Extractor();

        protected:

            /**
             * Calls methods to collect information about Memory.
             * Overrides virtual function from the base class
             */
            virtual data collect() override;
            hwloc_topology_t topology;

        private:
            static Registrar registrar;
            
            /**
             * Creates hwloc_obj_t and returns Memory information into the structure "result"
             * @param result Reference to data object, gets filled with information
             */
            void collect_memory_info(data& result);
    };    

}} /* closing namespace sysmap::hwloc */

#endif /* __SYSMAP_HWLOC_MEMORY_INFO_EXTRACTOR_HPP__ */
