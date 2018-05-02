#include "utils.hpp"

#include <sqlite_modern_cpp.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

#include "output.hpp"

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <map>

using namespace sysmap;
namespace po = boost::program_options;

std::map<int, std::vector<int>> getHostIDs(const std::string dbname, const std::vector<std::string>& names = {}){
    sqlite::database db(dbname);
    std::map<int, std::vector<int>> results;

    //returns std::vector<int> of DIDs for the given HostID
    auto getDIDofHost = [&](int HostID){
        std::vector<int> DIDs;
        db << "select DID from Host2Data where HostID = ? ;"
           << HostID
           >> [&](int DID) {
               DIDs.push_back(DID);
           };
        return std::move(DIDs);
    };

    //fills resuls with HostID : { DID, DID, DID, ... }
    auto fillHostID = [&](int HostID){
        results[HostID] = getDIDofHost(HostID);
    };

    if(names.size() == 0){
        db << "select HostID from Hosttable;"
             >> fillHostID;
        return std::move(results);
    }

    for(const auto name : names){
        db << "select HostID from Hosttable where Hostname = ? ;"
           << name
           >> fillHostID;
    }
    return std::move(results);
}

std::vector<int> getEIDs(const std::string dbname, const std::vector<std::string>& names = {}){
    std::vector<int> results;
    sqlite::database db(dbname);

    auto fillEID = [&results](int EID){
        results.push_back(EID);
    };

    if(names.size() == 0){
        db << "select EID from Extractortable;"
             >> fillEID;
        return std::move(results);
    }

    for(const auto name : names){
        db << "select EID from Extractortable where Name = ? ;"
           << name
           >> fillEID;
    }
    return std::move(results);
}

void queryToJSON(std::ostream& jsonstring, std::string filename, const std::map<int, std::vector<int>>& hosts, const std::vector<int>& extractors){
    sqlite::database db(filename);
    rapidjson::OStreamWrapper osw(jsonstring);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

    auto getHostName = [&db](int hostid){
        std::string name;
        db << "select Hostname from Hosttable where HostID = ? ;"
           << hostid
           >> name;
        return std::move(name);
    };

    auto getExtractorName = [&db](int EID){
        std::string name;
        db << "select Name from Extractortable where EID = ? ;"
           << EID
           >> name;
        return std::move(name);

    };

    auto getData = [&db](int EID, int DID){
        std::string data("");

        try {
            db << "select Data from Datatable where (EID, DID) = (?, ?);"
               << EID
               << DID
               >> data;
        }
        //Catches err if query returns nothing
        catch (sqlite::sqlite_exception& e) {
            /* std::cerr  << e.get_code() << ": " << e.what() << " during " */
                 /* << e.get_sql() << std::endl; */
        }

        return std::move(data);
    };

    writer.StartObject();
    for(const auto host : hosts){
        writer.Key(getHostName(host.first).c_str());
        writer.StartArray();
        for(const auto eid : extractors){
            for(const auto did : host.second){
                auto data = getData(eid, did);
                if(data.size() != 0){
                    writer.StartArray();
                    writer.String(getExtractorName(eid).c_str());
                    writer.String(data.c_str());
                    writer.EndArray();
                }
            }
        }
        writer.EndArray();
    }
    writer.EndObject();
}

void listHosts(std::ostream& os, std::string dbname){
    sqlite::database db(dbname);
    db << "select Hostname from Hosttable;"
       >> [&](std::string Hostname){
           os << Hostname << std::endl;
       };
}

void listExtractors(std::ostream& os, std::string dbname){
    sqlite::database db(dbname);
    db << "select Name from Extractortable;"
       >> [&](std::string Extractorname){
           os << Extractorname << std::endl;
       };
}

int main(int argc, char** argv)
{

    po::options_description description("sysquery - query sysmap.db");

    description.add_options()
        ("help,h", "Display help message")
        ("filename,f", po::value<std::string>()->required(), "Filename of the database file")
        ("output,o", po::value<std::string>(), "If specified, output will be written to given file")
        ("nodes,n", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given hosts")
        ("extractors,e", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given extractors")
        ("query-string,q", po::value<std::string>(), "Query String")
        ("listHosts,l", "Lists all Hostnames found in database")
        ("listExtractors,L", "Lists all Extractors found in database");

    po::variables_map vm;
    std::string filename, query_string;
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

        if (vm.count("query-string")){
            query_string = vm["query-string"].as<std::string>();
        }

        if (vm.count("listHosts")){
            listHosts(out, filename);
            exit(1);
        }
        if (vm.count("listExtractors")){
            listExtractors(out, filename);
            exit(1);
        }

    }
    catch (const po::error& ex) {
        utils::log::logging::error() << "[sysquery] Error, recieved Exception: " << ex.what() << "\n";
        return -1;
    }

    utils::log::logging::debug() << "[sysquery] opened database file";

    auto host_map = getHostIDs(filename, hosts);
    utils::log::logging::debug() << "[sysquery] host_map filled with data";
    auto extr_vec = getEIDs(filename, extractors);
    utils::log::logging::debug() << "[sysquery] extr_vec filled with data";

    queryToJSON(out, filename, host_map, extr_vec);

    return 0;
}
