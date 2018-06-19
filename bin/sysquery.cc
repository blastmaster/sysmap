#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <sqlite_modern_cpp.h>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

#include "output.hpp"
#include "utils.hpp"
#include "sysquery.hpp"

using namespace sysmap;
namespace po = boost::program_options;

int main(int argc, char** argv)
{

    po::options_description description("sysquery - query sysmap.db");

    description.add_options()
        ("help,h", "Display help message")
        ("filename,f", po::value<std::string>(), "Filename of the database file")
        ("logfile,F", po::value<std::string>(), "Name of the Logfile")
        ("output,o", po::value<std::string>(), "If specified, output will be written to given file")
        ("nodes,n", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given hosts")
        ("extractors,e", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given extractors")
        ("listHosts,l", "Lists all Hostnames found in database")
        ("listExtractors,L", "Lists all Extractors found in database");

    po::variables_map vm;
    std::string dbfile;
    std::string logfile = "log.txt";
    std::vector<std::string> hosts {};
    std::vector<std::string> extractors {};
    outwrapper out;

    try
    {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << description;
            return 0;
        }

        if (vm.count("filename")) {
            dbfile = vm["filename"].as<std::string>();

            //check if given file exists
            std::ifstream file(dbfile);
            if(!file.good()){
                utils::log::logging::debug() <<
                    "[sysquery] given database file doesnt exist or is corrupted. exiting...";
                return -1;
            }
            file.close();

            utils::log::logging::debug() <<
                "[sysquery] set actual database dbfile to: [" << dbfile << "]";
        } else {
            utils::log::logging::debug() << "No Database file was specified. Aborting...";
            return 0;
        }

        if (vm.count("logfile")) {
            logfile = vm["logfile"].as<std::string>();
        }

        if (vm.count("output")) {
            auto fname = vm["output"].as<std::string>();
            out.set_file(fname);
            utils::log::logging::debug() << "[sysquery] Setting output to: [" << fname << "]\n";
        }
        else {
            utils::log::logging::debug() << "[sysquery] Setting output to stdout.\n";
        }

        if (vm.count("nodes")){
            hosts = vm["nodes"].as<std::vector<std::string>>();
            for (const auto& host : hosts) {
                utils::log::logging::debug() << "[sysquery] host: " << host << " defined";
            }
        }

        if (vm.count("extractors")){
            extractors = vm["extractors"].as<std::vector<std::string>>();
            for (const auto& extr : extractors) {
                utils::log::logging::debug() << "[sysquery] extr: " << extr << " defined";
            }
        }

        if (vm.count("listHosts")){
            listHosts(out, dbfile);
            return 0;
        }
        if (vm.count("listExtractors")){
            listExtractors(out, dbfile);
            return 0;
        }

    }
    catch (const po::error& ex) {
        utils::log::logging::error() << "[sysquery] Error, recieved Exception: " << ex.what() << "\n";
        return -1;
    }

    //collect HostIDs and their DIDs
    sqlite::database db(dbfile);
    const auto host_map = getHostIDs(db, hosts);
    utils::log::logging::debug() << "[sysquery] host_map filled with data";
    //collect all EIDs
    const auto extr_vec = getEIDs(db, extractors);
    utils::log::logging::debug() << "[sysquery] extr_vec filled with data";

    //query DB and write to cout or file
    queryToJSON(out, db, host_map, extr_vec);

    return 0;
}
