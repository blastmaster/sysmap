#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "extractor_set.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "linux/infiniband_extractor.hpp"
#include "hwloc/hwloc_extractor.hpp"
#include "output.hpp"
#include "utils.hpp"

using namespace adafs;

namespace po = boost::program_options;

/**
 * Options:
 * help, version {on/off}
 * list available extractors {on/off}
 * define extractors to use {list of strings}
 * just run node-local extractors.{on/off}
 * extract network, or just define extractor explicitly {on/off}
 * output options {JSON|XML|ASCII|SQLITE3}
 **/

int main(int argc, char** argv)
{

    po::options_description description("sysmap - gather topological information about HPC-Systems.");

    description.add_options()
        ("help,h", "Display help message")
        ("version", "Display version number")
        ("list,l", "List all available extractors")
        ("format,f", po::value<std::string>()->default_value("JSON"), "Output format")
        ("output,o", po::value<std::string>(), "Ouput file, default to stdout.")
        ("extractor,e", po::value<std::vector<std::string>>()->multitoken()->composing(), "Define which extractors are used.");

    po::variables_map vm;

    try
    {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);

        auto fmt = Output_format::JSON;
        if (vm.count("help")) {
            std::cout << description;
            return 0;
        }
        if (vm.count("format")) {
            auto fmt_arg = vm["format"].as<std::string>();
            if (boost::iequals(fmt_arg, "XML")) {
                fmt = Output_format::XML;
                utils::log::logging::debug() << "[sysmap] setting format to: XML\n";
            }
            else if (boost::iequals(fmt_arg, "ASCII")) {
                fmt = Output_format::ASCII;
                utils::log::logging::debug() << "[sysmap] setting format to: ASCII\n";
            }
            else if (boost::iequals(fmt_arg, "JSON")) {
                fmt = Output_format::JSON;
                utils::log::logging::debug() << "[sysmap] setting format to: JSON\n";
            }
            else {
                utils::log::logging::error() << "[sysmap] Error, format argument: [" << fmt_arg << "] not valid!\n";
            }
        }

        std::ostream out(std::cout.rdbuf()); // set default to std::cout
        if (vm.count("output")) {
            auto output_arg = vm["output"].as<std::string>();
            std::ofstream of(output_arg);
            std::ostream out(of.rdbuf());
            utils::log::logging::debug() << "[sysmap] Setting output to: [" << output_arg << "]\n";
        }
        else {
            utils::log::logging::debug() << "[sysmap] Setting output to stdout.\n";
        }

        if (vm.count("list")) {
            // TODO: list all available extractors

        }

        if (vm.count("extractor")) {
            // TODO: recieve list of extractors to use!
            auto extrs = vm["extractor"].as<std::vector<std::string>>();
            for (const auto& ext : extrs) {
                utils::log::logging::debug() << "[sysmap] extractor: " << ext << " defined\n";
            }
        }
    }
    catch (const po::error& ex) {
        utils::log::logging::error() << "[sysmap] Error, recieved Exception: " << ex.what() << "\n";
        return -1;
    }

    Extractor_Set extr_set {};

    extr_set.add_extractor("Filesystem", std::make_shared<linux::Filesystem_Extractor>());
    extr_set.add_extractor("Disks", std::make_shared<linux::Disk_Extractor>());
    extr_set.add_extractor("Hwloc", std::make_shared<hwloc::Hwloc_Extractor>());
    extr_set.add_extractor("Infiniband", std::make_shared<linux::Infiniband_Extractor>());
    //extr_set.extract();
    //extr_set.write(out, Output_format::XML);


    return 0;
}
