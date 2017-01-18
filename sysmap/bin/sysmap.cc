#include "extractor_set.hpp"
#include "linux/filesystem_extractor.hpp"
#include "linux/disk_extractor.hpp"
#include "linux/infiniband_extractor.hpp"
#include "hwloc/hwloc_extractor.hpp"
#include "output.hpp"
#include "utils.hpp"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>


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

    Extractor_Set extr_set {};
    auto fmt = Output_format::JSON;         // set default to JSON
    outwrapper out;

    // TODO better exception handling this coarse grained handling sucks!
    try
    {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);

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

        if (vm.count("output")) {
            auto output_arg = vm["output"].as<std::string>();
            out.set_file(output_arg);
            utils::log::logging::debug() << "[sysmap] Setting output to: [" << output_arg << "]\n";
        }
        else {
            utils::log::logging::debug() << "[sysmap] Setting output to stdout.\n";
        }

        if (vm.count("list")) {
            // list all available extractors
            std::cout << "Available Extractors:\n";
            for (const auto& kvp : Extractor::registry()) {
                std::cout << "Extractor: " << kvp.first << "\n";
            }
        }

        if (vm.count("extractor")) {
            auto extrs = vm["extractor"].as<std::vector<std::string>>();
            for (const auto& ext : extrs) {
                utils::log::logging::debug() << "[sysmap] extractor: " << ext << " defined\n";
                extr_set.add_by_tag(ext);
            }
        }
        else { // TODO provide useful default!
            utils::log::logging::debug() << "[sysmap] no extractor specified!\n";
            return 1;
        }
    }
    catch (const po::error& ex) {
        utils::log::logging::error() << "[sysmap] Error, recieved Exception: " << ex.what() << "\n";
        return -1;
    }

    extr_set.extract();
    extr_set.write(out, fmt);

    return 0;
}
