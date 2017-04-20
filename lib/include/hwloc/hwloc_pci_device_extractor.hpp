#ifndef __ADAFS_HWLOC_PCI_DEVICE_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_PCI_DEVICE_EXTRACTOR_HPP__

#include "../extractors/pci_device_extractor.hpp"

extern "C"{
#include <hwloc.h>
}


namespace adafs { namespace hwloc {

    /**
     *@class Hwloc_PCI_Device_Extractor
     */
    
    struct Hwloc_PCI_Device_Extractor : extractor::PCI_Device_Extractor
    {

        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_PCI_Device_Extractor>(); }
        
        /**
         * Constructor
         * Initializes, sets flags and loads hwloc topology
         */
        Hwloc_PCI_Device_Extractor();

        /**
         * Virtual Destructor
         * Destroys hwloc topology
         */
        virtual ~Hwloc_PCI_Device_Extractor();

        protected:
            
            /**
             * Calls methods to collect information about PCI Devices.
             * Overrides virtual function from the base class
             */
            virtual data collect() override;
            hwloc_topology_t topology;

       
        private:
            static Registrar registrar;
            
            /**
             * Creates hwloc_obj_t and returns PCI Device information into the structure "result"
             * @param result Reference to data object, gets filled with information
             */
            void collect_pci_devices(data& result);

            PCI_Device assemble_pci_device(const std::string& name,
                    const hwloc_obj_t& obj) const;
    };    


}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_PCI_DEVICE_EXTRACTOR_HPP__*/
