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

using namespace sysmap;
namespace po = boost::program_options;

//mapping HostIDs to all DataIDs (DIDs) connected to that Host
using hostMap_t = std::map<int, std::vector<int>>;

/**
 * Get a Map of HostsIDs linked to their DataIDs for the given Hosts.
 * Needed to call queryToJSON(...)
 *
 * @param db Database Object
 * @param names Vector of Hostnames
 * @return Returns hostMap_t
 */
hostMap_t getHostIDs(sqlite::database& db, const std::vector<std::string>& names = {}){
    hostMap_t results;

    //returns std::vector<int> of DIDs for the given HostID
    auto getDIDofHost = [&](int HostID){
        std::vector<int> DIDs;
        db << "select DID from Host2Data where HostID = ? ;"
           << HostID
           >> [&DIDs](int DID) {
               DIDs.push_back(DID);
           };
        return DIDs;
    };

    //fills resuls with HostID : { DID, DID, DID, ... }
    auto fillHostID = [&](int HostID){
        results[HostID] = getDIDofHost(HostID);
    };

    if(names.size() == 0){
        db << "select HostID from Hosttable;"
             >> fillHostID;
        return results;
    }

    for(const auto name : names){
        db << "select HostID from Hosttable where Hostname = ? ;"
           << name
           >> fillHostID;
    }
    return results;
}

/**
 * Get a Vector of ExtractorIDs for the given Hosts
 * Needed to call queryToJSON(...)
 *
 * @param db Database Object
 * @param names Vector of Hostnames
 * @return Returns std::vector<int> with the EIDs
 */
std::vector<int> getEIDs(sqlite::database& db, const std::vector<std::string>& names = {}){
    std::vector<int> results;

    auto fillEID = [&results](int EID){
        results.push_back(EID);
    };

    if(names.size() == 0){
        db << "select EID from Extractortable;"
             >> fillEID;
        return results;
    }

    for(const auto name : names){
        db << "select EID from Extractortable where Name = ? ;"
           << name
           >> fillEID;
    }
    return results;
}

/**
 * Writes a JSON String to an ostream containing Data queried from the Database.
 * What actually is queried depends on the given hosts and extractors,
 * those should be gathered using getHostIDs(..) and getEIDs(..)
 *
 * @param jsonstring Ostream to which the JSON String is written to
 * @param db Database Object
 * @param hosts hostMap_t which should be gathered using getHostIDs(..)
 * @param extractors vector of ints which sould be gathered using getEIDs(..)
 */
void queryToJSON(std::ostream& jsonstring, sqlite::database& db, const hostMap_t& hosts, const std::vector<int>& extractors){
    rapidjson::OStreamWrapper osw(jsonstring);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

    auto getHostName = [&db](int hostid){
        std::string name;
        db << "select Hostname from Hosttable where HostID = ? ;"
           << hostid
           >> name;
        return name;
    };

    auto getExtractorName = [&db](int EID){
        std::string name;
        db << "select Name from Extractortable where EID = ? ;"
           << EID
           >> name;
        return name;

    };

    auto getData = [&db](int EID, int DID){
        std::string data;

        try {
            db << "select Data from Datatable where EID = ? and DID = ?;"
               << EID
               << DID
               >> data;
        }
        //Catches err if query returns nothing
        catch (sqlite::sqlite_exception& e) {
            utils::log::logging::error() << e.get_code() << ": " << e.what() << " during "
                  << e.get_sql() << " EID " << EID << " DID " << DID;
        }

        return data;
    };

    writer.StartObject();
    for(const auto& host : hosts){
        writer.Key(getHostName(host.first).c_str());
        writer.StartObject();
        for(const auto& eid : extractors){
            for(const auto& did : host.second){
                const auto data = getData(eid, did);
                if(data.size() != 0){
                    writer.Key(getExtractorName(eid).c_str());
                    //writer.RawValue(...) takes Raw JSON String and does not modify it like writer.String()
                    writer.RawValue(data.c_str(), data.size(), rapidjson::kStringType);
                }
            }
        }
        writer.EndObject();
    }
    writer.EndObject();
}

/**
 * Writes a list of every Hostname contained in the Database to the given ostream
 *
 * @param os Ostream to which the Hostnames will be written to
 * @param dbfile Path to the Database file
 */
void listHosts(std::ostream& os, const std::string& dbfile){
    sqlite::database db(dbfile);
    db << "select Hostname from Hosttable;"
       >> [&os](std::string Hostname){
           os << Hostname << std::endl;
       };
}

/**
 * Writes a list of every Extractorname contained in the Database to the given ostream
 *
 * @param os Ostream to which the Extractornames will be written to
 * @param dbfile Path to the Database file
 */
void listExtractors(std::ostream& os, const std::string& dbfile){
    sqlite::database db(dbfile);
    db << "select Name from Extractortable;"
       >> [&os](std::string Extractorname){
           os << Extractorname << std::endl;
       };
}

int main(int argc, char** argv)
{

    po::options_description description("sysquery - query sysmap.db");

    description.add_options()
        ("help,h", "Display help message")
        ("filename,f", po::value<std::string>(), "Filename of the database file")
        ("output,o", po::value<std::string>(), "If specified, output will be written to given file")
        ("nodes,n", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given hosts")
        ("extractors,e", po::value<std::vector<std::string>>()->multitoken()->composing(), "Only Query given extractors")
        ("listHosts,l", "Lists all Hostnames found in database")
        ("listExtractors,L", "Lists all Extractors found in database");

    po::variables_map vm;
    std::string dbfile;
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
