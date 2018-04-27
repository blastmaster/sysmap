#include "utils.hpp"

#include <sqlite_modern_cpp.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <map>

using namespace sysmap;
namespace po = boost::program_options;

std::vector<int> getHostIDs(const std::string dbname, const std::vector<std::string>& names = {}){
    sqlite::database db(dbname);
    utils::log::logging::debug() << 
        "[sysquery::getHostIDs] getting HostIDs...";
    std::vector<int> results;

    auto push_HostID = [&results](int HostID){
        results.push_back(HostID);
        std::cout << "Pusing back HostID: " << HostID << std::endl;
    };

    if(names.size() == 0){
        db << "select HostID from Hosttable;"
             >> push_HostID;
        return std::move(results);
    }

    for(const auto name : names){
        db << "select HostID from Hosttable where Hostname = ? ;"
           << name
           >> push_HostID;
    }
    return std::move(results);
}

int main(int argc, char** argv)
{

    po::options_description description("sysquery - query sysmap.db");

    description.add_options()
        ("help,h", "Display help message")
        ("filename,f", po::value<std::string>()->required(), "Filename of the database file")
        ("output,o", po::value<std::string>(), "If specified, output will be written to given file")
        ("nodes,n", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given hosts")
        ("query-string,q", po::value<std::string>(), "Query String");

    po::variables_map vm;
    std::string filename, query_string;
    std::vector<std::string> hosts {};

    try
    {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << description;
            return 0;
        }

        if (vm.count("filename")) {
            filename = vm["filename"].as<std::string>();

            //check if given file exists
            std::ifstream file(filename);
            if(!file.good()){
                utils::log::logging::debug() << 
                    "[sysquery] given database file doesnt exist or is corrupted. exiting...";
                return -1;
            }
            file.close();

            utils::log::logging::debug() << 
                "[sysquery] set actual database filename to: [" << filename << "]";
        }

        if (vm.count("nodes")){
            hosts = vm["nodes"].as<std::vector<std::string>>(); 
            for (const auto& host : hosts) {
                utils::log::logging::debug() << "[sysquery] host: " << host << " defined\n";
            }
        }

        if (vm.count("query-string")){
            query_string = vm["query-string"].as<std::string>();
        }

    }
    catch (const po::error& ex) {
        utils::log::logging::error() << "[sysquery] Error, recieved Exception: " << ex.what() << "\n";
        return -1;
    }

    utils::log::logging::debug() << "[sysquery] opened database file";
    /* std::stringstream sstr; */
    /* rapidjson::OStreamWrapper osw(os); */

    auto hostIDs = getHostIDs(filename, hosts);

    return 0;
}
