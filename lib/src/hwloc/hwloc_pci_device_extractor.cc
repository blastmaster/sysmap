#include "hwloc/hwloc_pci_device_extractor.hpp"
#include "utils.hpp"


namespace adafs { namespace hwloc {

    Registrar Hwloc_PCI_Device_Extractor::registrar("hwloc_pci", &Hwloc_PCI_Device_Extractor::create);

    Hwloc_PCI_Device_Extractor::data Hwloc_PCI_Device_Extractor::collect()
    {
        data result;
        collect_pci_devices(result);

        return result;
    }

   Hwloc_PCI_Device_Extractor::PCI_Device Hwloc_PCI_Device_Extractor::assemble_pci_device(const std::string& name,
            const hwloc_obj_t& obj) const
    {
        auto pcidev = obj->attr->pcidev;
        return PCI_Device {name, pcidev.domain, pcidev.bus,
            pcidev.dev, pcidev.func, pcidev.class_id,
            pcidev.vendor_id, pcidev.device_id,
            pcidev.subvendor_id, pcidev.subdevice_id, pcidev.revision,
            pcidev.linkspeed
        };
    }

    void Hwloc_PCI_Device_Extractor::collect_pci_devices(data& result)
    {
        hwloc_obj_t obj;
        auto num_pci_devs = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PCI_DEVICE);
        adafs::utils::log::logging::debug() << "hwloc pci device extractor number of detected pci devices: " << num_pci_devs;
        for (unsigned i = 0; i < num_pci_devs; ++i) {
            obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PCI_DEVICE, i);
            if (obj == nullptr) {
                adafs::utils::log::logging::error() << "ERROR! hwloc pci device extractor PCI device object is NULL!";
                continue;
            }
            std::string name((obj->name == nullptr) ? "None" : obj->name);
            if (&(obj->attr->pcidev) == nullptr) {
                adafs::utils::log::logging::error() << "ERROR! hwloc pci device extractor PCI device attributes are NULL!";
                continue;
            }

            result.pci_devices.push_back(assemble_pci_device(name, obj));
        }
    }

}}
