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
        machine->add("NodeName", make_value<String_value>(std::move(data.machine_info.nodename)));
        machine->add("Version", make_value<String_value>(std::move(data.machine_info.version)));
        machine->add("Release", make_value<String_value>(std::move(data.machine_info.release)));
        machine->add("Sysname", make_value<String_value>(std::move(data.machine_info.sysname)));
        machine->add("Machine", make_value<String_value>(std::move(data.machine_info.machine)));
        for (const auto& module : data.machine_info.modules){
            machine->add(module.first, make_value<Array_value>(module.second));
        }
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
