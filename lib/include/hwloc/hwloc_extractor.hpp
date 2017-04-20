#ifndef __ADAFS_HWLOC_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_EXTRACTOR_HPP__

#include "../extractors/hwloc_extractor.hpp"


namespace adafs { namespace hwloc {

    /**
     *@class Hwloc_Extractor
     */

    struct Hwloc_Extractor : extractor::Hwloc_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_Extractor>(); }

        protected:
            
            /**
             * Calls methods to collect information about the System.
             * Overrides virtual function from the base class
             */
            virtual data collect() override;

        private:
            static Registrar registrar;
            
            /**
             * Creates hwloc_obj_t and returns PCI_Device information into the structure "result"
             * @param result Reference to data object, gets filled with information
             */
            void collect_pci_devices(data& result);
            
            /**
             * Creates hwloc_obj_t and returns Machine information into the structure "result"
             * @param result Reference to data object, gets filled with information
             */
            void collect_machine_info(data& result);
            
            /**
             * Creates hwloc_obj_t and returns Memory information into the structure "result"
             * @param result Reference to data object, gets filled with information
             */
            void collect_memory_info(data& result);

            PCI_Device assemble_pci_device(const std::string& name,
                    const hwloc_obj_t& obj) const;

    };

}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_EXTRACTOR_HPP__ */
