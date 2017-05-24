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

        auto machine = make_value<Map_value>(); //contains uname information
        
        machine->add("NodeName", make_value<String_value>(std::move(data.system_info.nodename)));
        machine->add("Version", make_value<String_value>(std::move(data.system_info.version)));
        machine->add("Release", make_value<String_value>(std::move(data.system_info.release)));
        machine->add("Sysname", make_value<String_value>(std::move(data.system_info.sysname)));
        machine->add("Machine", make_value<String_value>(std::move(data.system_info.machine)));

        auto kernel = make_value<Map_value>(); //contains kernel config information
        for (const auto& config : data.kernel_config)
        {

            kernel->add(config.name, make_value<String_value>(boost::lexical_cast<std::string>(config.value)));
        }
        data.kernel_config.clear();

        auto modules = make_value<Array_value>(); //contains module information
        for (const auto& config : data.modules)
        {
            auto value = make_value<Map_value>();
            value->add("Name", make_value<String_value>(config.name));
            value->add("Size", make_value<Int_value>(config.size));
            value->add("Loaded", make_value<Int_value>(config.loaded));

            modules->add(std::move(value));
        }
        data.modules.clear();

        findings.add_info("machineinfo", std::move(machine));
        findings.add_info("kernelinfo", std::move(kernel));
        findings.add_info("moduleinfo", std::move(modules));


    }

    void Kernel_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
        utils::log::logging::error() << "[adafs::extractor::Kernel_Extractor] store() is not implemented yet";
    }
} }

