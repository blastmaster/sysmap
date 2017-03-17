#ifndef __ADAFS_HWLOC_PCI_DEVICE_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_PCI_DEVICE_EXTRACTOR_HPP__

#include "../extractors/pci_device_extractor.hpp"

extern "C"{
#include <hwloc.h>
}


namespace adafs { namespace hwloc {
    
    struct Hwloc_PCI_Device_Extractor : extractor::PCI_Device_Extractor
    {

        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_PCI_Device_Extractor>(); }
        
        Hwloc_PCI_Device_Extractor();

        virtual ~Hwloc_PCI_Device_Extractor();

        protected:
            virtual data collect() override;
            hwloc_topology_t topology;

       
        private:
            static Registrar registrar;
            void collect_pci_devices(data& result);
            PCI_Device assemble_pci_device(const std::string& name,
                    const hwloc_obj_t& obj) const;
    };    


}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_PCI_DEVICE_EXTRACTOR_HPP__*/
