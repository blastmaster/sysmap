#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"

#include "extractors/machine_info_extractor.hpp"

#include <sqlite_modern_cpp.h>

#include <cstdio>

using namespace sqlite;

namespace adafs { namespace extractor {

    void Machine_Info_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        auto machine = make_value<Map_value>();

        machine->add("ProductName", make_value<String_value>(std::move(data.machine_info.product_name)));
        machine->add("ProductVersion", make_value<String_value>(std::move(data.machine_info.product_version)));
        machine->add("BoardVendor", make_value<String_value>(std::move(data.machine_info.board_vendor)));
        machine->add("BoardName", make_value<String_value>(std::move(data.machine_info.board_name)));
        machine->add("BoardVersion", make_value<String_value>(std::move(data.machine_info.board_version)));
        machine->add("BoardAssetTag", make_value<String_value>(std::move(data.machine_info.board_asset_tag)));
        machine->add("ChassisVendor", make_value<String_value>(std::move(data.machine_info.chassi_vendor)));
        machine->add("ChassisType", make_value<String_value>(std::move(data.machine_info.chassi_type)));
        machine->add("ChassisVersion", make_value<String_value>(std::move(data.machine_info.chassi_version)));
        machine->add("ChassisAssetTag", make_value<String_value>(std::move(data.machine_info.chassi_asset_tag)));
        machine->add("BiosVendor", make_value<String_value>(std::move(data.machine_info.bios_vendor)));
        machine->add("BiosVersion", make_value<String_value>(std::move(data.machine_info.bios_version)));
        machine->add("BiosDate", make_value<String_value>(std::move(data.machine_info.bios_date)));
        machine->add("SysVendor", make_value<String_value>(std::move(data.machine_info.sys_vendor)));
        machine->add("LinuxCgroup", make_value<String_value>(std::move(data.machine_info.linux_cgroup)));
        machine->add("OsName", make_value<String_value>(std::move(data.machine_info.os_name)));
        machine->add("OsRelease", make_value<String_value>(std::move(data.machine_info.os_release)));
        machine->add("OsVersion", make_value<String_value>(std::move(data.machine_info.os_version)));
        machine->add("Hostname", make_value<String_value>(std::move(data.machine_info.hostname)));
        machine->add("Architecture", make_value<String_value>(std::move(data.machine_info.architecture)));

        findings.add_info("machineinfo", std::move(machine));

    }

    void Machine_Info_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
        //TODO: fill out store fct
        auto m_info = findings.get<Map_value>("MachineInfo");
//        std::string hostname = m_info->get<String_value>("Hostname")->value();
//        std::string kernel_version = m_info->get<String_value>("Kernel_Version")->value();
//        std::map<std::string, std::string> kernel_modules = m_info->get<Map_value>("Kernel_Modules")->value();
        adafs::utils::log::logging::debug() << "machine info extractor insterted machine info";

    }

}} /* closing namesapce adafs::extractor */
