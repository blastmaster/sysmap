#ifndef __ADAFS_HWLOC_MACHINE_INFO_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_MACHINE_INFO_EXTRACTOR_HPP__

#include "../extractors/hwloc_extractor.hpp"

extern "C"{
#include <hwloc.h>
}


namespace adafs { namespace hwloc {

    /**
     *@class Hwloc_Machine_Info_Extractor
     */

    struct Hwloc_Machine_Info_Extractor : extractor::Hwloc_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_Machine_Info_Extractor>(); }
        
        /**
         * Constructor
         * Initializes, sets flags and loads hwloc topology
         */
        Hwloc_Machine_Info_Extractor();
        
        /**
         * Virtual Destructor
         * Destroys hwloc topology
         */
        virtual ~Hwloc_Machine_Info_Extractor();

        protected:

            /**
             * Calls methods to collect information about the Machine.
             * Overrides virtual function from the base class
             */
            virtual data collect() override;

            hwloc_topology_t topology;

        private:
            static Registrar registrar;

            /**
             * Creates hwloc_obj_t and returns Machine information into the structure "result"
             * @param result Reference to data object, gets filled with information
             */
            void collect_machine_info(data& result);

    };

}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_MACHINE_INFO_EXTRACTOR_HPP__ */
