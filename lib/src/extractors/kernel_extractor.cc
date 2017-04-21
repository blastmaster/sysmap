#include "value.hpp"
#include "scalar_value.hpp"
#include "array_value.hpp"
#include "map_value.hpp"
#include "utils.hpp"
#include "boost/lexical_cast.hpp"

#include "extractors/kernel_extractor.hpp"

namespace adafs { namespace extractor {

    void Kernel_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();
        auto machine = make_value<Map_value>();
        
        machine->add("NodeName", make_value<String_value>(std::move(data.system_info.nodename)));
        machine->add("Version", make_value<String_value>(std::move(data.system_info.version)));
        machine->add("Release", make_value<String_value>(std::move(data.system_info.release)));
        machine->add("Sysname", make_value<String_value>(std::move(data.system_info.sysname)));
        machine->add("Machine", make_value<String_value>(std::move(data.system_info.machine)));

        auto kernel = make_value<Map_value>();
        for (const auto& config : data.kernel_config)
        {
            kernel->add(config.name, make_value<String_value>(boost::lexical_cast<std::string>(config.value)));
        }

        auto modules = make_value<Map_value>();
        for (const auto& config : data.modules)
        {
//            int module_information[2] = { config.size, config.loaded }
            modules->add(config.name, make_value<Int_value>(config.size));
        }

        findings.add_info("machine_info", std::move(machine));
        findings.add_info("kernel_info", std::move(kernel));
        findings.add_info("module_info", std::move(modules));


    }

    void Kernel_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
    }
} }

