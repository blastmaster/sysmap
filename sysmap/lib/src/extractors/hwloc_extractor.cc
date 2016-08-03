#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"

#include "extractors/hwloc_extractor.hpp"


namespace adafs { namespace extractor {

    Hwloc_Extractor::Hwloc_Extractor() : Extractor("Hwloc")
    {
        hwloc_topology_init(&topology);
        hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_IO);
        hwloc_topology_load(topology);
    }

    Hwloc_Extractor::~Hwloc_Extractor()
    {
        hwloc_topology_destroy(topology);
    }

    void Hwloc_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        auto machine = make_value<Map_value>();

        machine->add("DMIProductName", make_value<String_value>(std::move(data.machine_info.product_name)));
        machine->add("DMIProductVersion", make_value<String_value>(std::move(data.machine_info.product_version)));
        machine->add("DMIBoardVendor", make_value<String_value>(std::move(data.machine_info.board_vendor)));
        machine->add("DMIBoardName", make_value<String_value>(std::move(data.machine_info.board_name)));
        machine->add("DMIBoardVersion", make_value<String_value>(std::move(data.machine_info.board_version)));
        machine->add("DMIBoardAssetTag", make_value<String_value>(std::move(data.machine_info.board_asset_tag)));
        machine->add("DMIChassisVendor", make_value<String_value>(std::move(data.machine_info.chassi_vendor)));
        machine->add("DMIChassisType", make_value<String_value>(std::move(data.machine_info.chassi_type)));
        machine->add("DMIChassisVersion", make_value<String_value>(std::move(data.machine_info.chassi_version)));
        machine->add("DMIChassisAssetTag", make_value<String_value>(std::move(data.machine_info.chassi_asset_tag)));
        machine->add("DMIBIOSVendor", make_value<String_value>(std::move(data.machine_info.bios_vendor)));
        machine->add("DMIBIOSVersion", make_value<String_value>(std::move(data.machine_info.bios_version)));
        machine->add("DMIBIOSDate", make_value<String_value>(std::move(data.machine_info.bios_date)));
        machine->add("DMISysVendor", make_value<String_value>(std::move(data.machine_info.sys_vendor)));
        // NOTE: hwloc_backend will not be added.
        machine->add("LinuxCgroup", make_value<String_value>(std::move(data.machine_info.linux_cgroup)));
        machine->add("OSName", make_value<String_value>(std::move(data.machine_info.os_name)));
        machine->add("OSRelease", make_value<String_value>(std::move(data.machine_info.os_release)));
        machine->add("OSVersion", make_value<String_value>(std::move(data.machine_info.os_version)));
        machine->add("Hostname", make_value<String_value>(std::move(data.machine_info.hostname)));
        machine->add("Architecture", make_value<String_value>(std::move(data.machine_info.architecture)));
        // NOTE: hwloc_version will not be added.
        machine->add("ProcessName", make_value<String_value>(std::move(data.machine_info.process_name)));

        findings.add_info("MachineInfo", std::move(machine));

        auto memory = make_value<Map_value>();

        memory->add("TotalMemory", make_value<Uint_value>(data.memory_info.total_memory));
        memory->add("LocalMemory", make_value<Uint_value>(data.memory_info.local_memory));

        auto pages = make_value<Array_value>();
        for (const auto& page : data.memory_info.page_types) {
            auto value = make_value<Map_value>();

            value->add("PageSize", make_value<Uint_value>(page.size));
            value->add("Count", make_value<Uint_value>(page.count));

            pages->add(std::move(value)); //map_value in array_value does this crash xml output???
        }
        memory->add("PageTypes", std::move(pages));
        findings.add_info("MemoryInfo", std::move(memory));

        //TODO pci devices

    }

}} /* closing namesapce adafs::extractor */
