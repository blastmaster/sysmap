#ifndef __ADAFS_HWLOC_EXTRACTOR_HPP__
#define __ADAFS_HWLOC_EXTRACTOR_HPP__

#include "../extractors/pci_device_extractor.hpp"

namespace adafs { namespace hwloc {
    
    struct Hwloc_PCI_Device_Extractor : extractor::PCI_Device_Extractor
    {
        static std::unique_ptr<Extractor> create() { return std::make_unique<Hwloc_PCI_Device_Extractor>(); }

        protected:
              virtual data collect() override;
    
        private:
              static Registrar registrar;
              void collect_pci_devices(data& result);
              PCI_Device assemble_pci_device(const std::string& name,
                      const hwloc_obj_t& obj) const;
    };    


}} /* closing namespace adafs::hwloc */

#endif /* __ADAFS_HWLOC_EXTRACTOR_HPP__*/
