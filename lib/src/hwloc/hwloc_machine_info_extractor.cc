#include "hwloc/hwloc_machine_info_extractor.hpp"
#include "utils.hpp"


namespace adafs { namespace hwloc {

    Registrar Hwloc_Machine_Info_Extractor::registrar("hwloc_machine", &Hwloc_Machine_Info_Extractor::create);

    Hwloc_Machine_Info_Extractor::data Hwloc_Machine_Info_Extractor::collect()
    {
        data result;
        collect_machine_info(result);

        return result;
    }


    void Hwloc_Machine_Info_Extractor::collect_machine_info(data& result)
    {
        hwloc_obj_t root_obj = hwloc_get_root_obj(topology);
        if (!root_obj) {
            utils::log::logging::error() << "Error: no hwloc root obj!\n";
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
        result.machine_info.linux_cgroup = hwloc_obj_get_info_by_name(root_obj, "LinuxCgroup");
        result.machine_info.os_name = hwloc_obj_get_info_by_name(root_obj, "OSName");
        result.machine_info.os_release = hwloc_obj_get_info_by_name(root_obj, "OSRelease");
        result.machine_info.os_version = hwloc_obj_get_info_by_name(root_obj, "OSVersion");
        result.machine_info.hostname = hwloc_obj_get_info_by_name(root_obj, "HostName");
        result.machine_info.architecture = hwloc_obj_get_info_by_name(root_obj, "Architecture");
    }
    
    Hwloc_Machine_Info_Extractor::Hwloc_Machine_Info_Extractor()
    {
        hwloc_topology_init(&topology);
        hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
        hwloc_topology_load(topology);
    }

    Hwloc_Machine_Info_Extractor::~Hwloc_Machine_Info_Extractor()
    {
        hwloc_topology_destroy(topology);
    }

}}
