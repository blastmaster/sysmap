#include <ctime>
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

//mapping HostIDs to all DataIDs (DIDs) connected to that Host
using hostMap_t = std::map<int, std::vector<int>>;

static void log(std::string data){
    std::ofstream logfile("sysquery_log.txt", std::ios_base::app);
    logfile << data << std::endl;
    logfile.close();
}

/**
 * Get a Map of HostsIDs linked to their DataIDs for the given Hosts.
 * Needed to call queryToJSON(...)
 *
 * @param db Database Object
 * @param names Vector of Hostnames
 * @return Returns hostMap_t
 */
static hostMap_t getHostIDs(sqlite::database& db,
                            const std::vector<std::string>& names = {}){
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
static std::vector<int> getEIDs(sqlite::database& db,
                                const std::vector<std::string>& names = {}){
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
static void queryToJSON(std::ostream& jsonstring,
                        sqlite::database& db,
                        const hostMap_t& hosts,
                        const std::vector<int>& extractors){
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
            std::stringstream error_log;
            error_log << time(0) << " - queryToJSON(..) - " << e.get_code() << ": " << e.what() << " during "
                  << e.get_sql() << " EID " << EID << " DID " << DID;
            log(error_log.str());
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
    jsonstring << std::endl;
}

/**
 * Writes a list of every Hostname contained in the Database to the given ostream
 *
 * @param os Ostream to which the Hostnames will be written to
 * @param dbfile Path to the Database file
 */
static void listHosts(std::ostream& os,
                      const std::string& dbfile){
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
static void listExtractors(std::ostream& os,
                           const std::string& dbfile){
    sqlite::database db(dbfile);
    db << "select Name from Extractortable;"
       >> [&os](std::string Extractorname){
           os << Extractorname << std::endl;
       };
}

