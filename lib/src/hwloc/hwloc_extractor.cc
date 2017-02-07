#include "hwloc/hwloc_extractor.hpp"
#include "utils.hpp"


namespace adafs { namespace hwloc {

    Hwloc_Extractor::data Hwloc_Extractor::collect()
    {
        data result;
        collect_machine_info(result);
        collect_memory_info(result);
        collect_pci_devices(result);

        return result;
    }


    void Hwloc_Extractor::collect_machine_info(data& result)
    {
        hwloc_obj_t root_obj = hwloc_get_root_obj(topology);
        if (!root_obj) {
            adafs::utils::log::logging::error() << "Error: no hwloc root obj!\n";
            return;
        }

        result.machine_info.product_name = hwloc_obj_get_info_by_name(root_obj, "DMIProductName");
        result.machine_info.product_version = hwloc_obj_get_info_by_name(root_obj, "DMIProductVersion");
        result.machine_info.board_vendor = hwloc_obj_get_info_by_name(root_obj, "DMIBoardVendor");
        result.machine_info.board_name = hwloc_obj_get_info_by_name(root_obj, "DMIBoardName");
        result.machine_info.board_version = hwloc_obj_get_info_by_name(root_obj, "DMIBoardVersion");
        result.machine_info.board_asset_tag = hwloc_obj_get_info_by_name(root_obj, "DMIBoardAssetTag");
        result.machine_info.chassi_vendor = hwloc_obj_get_info_by_name(root_obj, "DMIChassisVendor");
        result.machine_info.chassi_type = hwloc_obj_get_info_by_name(root_obj, "DMIChassisType");
        result.machine_info.chassi_version = hwloc_obj_get_info_by_name(root_obj, "DMIChassisVersion");
        result.machine_info.chassi_asset_tag = hwloc_obj_get_info_by_name(root_obj, "DMIChassisAssetTag");
        result.machine_info.bios_vendor = hwloc_obj_get_info_by_name(root_obj, "DMIBIOSVendor");
        result.machine_info.bios_version = hwloc_obj_get_info_by_name(root_obj, "DMIBIOSVersion");
        result.machine_info.bios_date = hwloc_obj_get_info_by_name(root_obj, "DMIBIOSDate");
        result.machine_info.sys_vendor = hwloc_obj_get_info_by_name(root_obj, "DMISysVendor");
        result.machine_info.hwloc_backend = hwloc_obj_get_info_by_name(root_obj, "Backend");
        result.machine_info.linux_cgroup = hwloc_obj_get_info_by_name(root_obj, "LinuxCgroup");
        result.machine_info.os_name = hwloc_obj_get_info_by_name(root_obj, "OSName");
        result.machine_info.os_release = hwloc_obj_get_info_by_name(root_obj, "OSRelease");
        result.machine_info.os_version = hwloc_obj_get_info_by_name(root_obj, "OSVersion");
        result.machine_info.hostname = hwloc_obj_get_info_by_name(root_obj, "HostName");
        result.machine_info.architecture = hwloc_obj_get_info_by_name(root_obj, "Architecture");
        result.machine_info.hwloc_version = hwloc_obj_get_info_by_name(root_obj, "hwlocVersion");
        result.machine_info.process_name = hwloc_obj_get_info_by_name(root_obj, "ProcessName");

    }


    void Hwloc_Extractor::collect_memory_info(data& result)
    {
        // NOTE: just works on machine level, not sure if this is always
        // correct, if not, search the whole topology for memory information
        // and just start with the root obj.
        hwloc_obj_t root_obj = hwloc_get_root_obj(topology);
        if (!root_obj) {
            adafs::utils::log::logging::error() << "Error: no hwloc root obj!\n";
            return;
        }

        result.memory_info.total_memory = root_obj->memory.total_memory;
        result.memory_info.local_memory = root_obj->memory.local_memory;

        for (int i = 0; i < root_obj->memory.page_types_len; ++i) {
            result.memory_info.page_types.emplace_back(root_obj->memory.page_types[i].size,
                        root_obj->memory.page_types[i].count);
        }
    }

    Hwloc_Extractor::PCI_Device Hwloc_Extractor::assemble_pci_device(const std::string& name,
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

    void Hwloc_Extractor::collect_pci_devices(data& result)
    {
        hwloc_obj_t obj;
        auto num_pci_devs = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PCI_DEVICE);
        adafs::utils::log::logging::debug() << "hwloc extractor number of detected pci devices: " << num_pci_devs;
        for (unsigned i = 0; i < num_pci_devs; ++i) {
            obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PCI_DEVICE, i);
            if (obj == nullptr) {
                adafs::utils::log::logging::error() << "ERROR! hwloc extractor PCI device object is NULL!";
                continue;
            }
            std::string name {obj->name};
            if (&(obj->attr->pcidev) == nullptr) {
                adafs::utils::log::logging::error() << "ERROR! hwloc extractor PCI device attributes are NULL!";
                continue;
            }

            result.pci_devices.push_back(assemble_pci_device(name, obj));
        }
    }

}} /* closing namespace adafs::hwloc */
