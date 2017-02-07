#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"
#include "utils.hpp"

#include "extractors/infiniband_extractor.hpp"

#include <sqlite_modern_cpp.h>

using namespace sqlite;

namespace adafs { namespace extractor {

    static void create_host_tab(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS hosts (\
                guid string PRIMARY KEY NOT NULL,\
                nports integer,\
                description string\
            );";

    }

    static void create_switch_tab(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS switches (\
                guid string PRIMARY KEY NOT NULL,\
                nports integer,\
                description string,\
                port_type string\
            );";
    }

    /*
    * represent connections m:n
    *
    * host
    * guid pk
    *
    * switch
    * guid pk
    *
    * con
    * to_guid pk
    * from_guid pk
    *
    * select description from host where guid = (select to_guid from con where from_guid = ?)
    */

    static void create_con_tab(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS connections (\
                    from_guid string NOT NULL,\
                    to_guid string NOT NULL,\
                    from_port int NOT NULL, \
                    to_port int NOT_NULL, \
                    PRIMARY KEY(from_guid, to_guid, from_port, to_port)\
                );";
    }


    static void create_con_details_tab(database& db)
    {
        db << "CREATE TABLE IF NOT EXISTS connection_details (\
                    local_port integer,\
                    peer_guid string,\
                    peer_port integer,\
                    peer_port_guid string,\
                    peer_description string,\
                    peer_lid integer,\
                    local_lid integer,\
                    local_lmc integer,\
                    speed string\
                );";
    }

    static void insert_connection(const std::string& from_guid,
                                const std::string& to_guid,
                                const uint64_t from_port,
                                const uint64_t to_port, database& db)
    {
        //TODO: prepared statement?
        db << "INSERT INTO connections VALUES (?, ?, ?, ?);"
            << from_guid << to_guid << (int) from_port << (int) to_port;
    }


    static void insert_connection_details(const uint64_t local_port,
                                        const std::string& peer_guid,
                                        const uint64_t peer_port,
                                        const std::string& peer_port_guid,
                                        const std::string& peer_description,
                                        const uint64_t peer_lid,
                                        const uint64_t local_lid,
                                        const uint64_t local_lmc,
                                        const std::string& speed, database& db)
    {
        //TODO: prepared statement?
        db << "INSERT INTO connection_details VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"
            << (int) local_port
            << peer_guid
            << (int) peer_port
            << peer_port_guid
            << peer_description
            << (int) peer_lid
            << (int) local_lid
            << (int) local_lmc
            << speed;
    }


    static void insert_switch(const std::string& guid,
                            const std::string& desc,
                            const std::string& port_type,
                            const uint64_t num_ports, database& db)
    {
        //TODO: prepared statement?
        db << "INSERT INTO switches VALUES (?, ?, ?, ?);"
            << guid
            << (int) num_ports
            << desc
            << port_type;
    }


    static void insert_host(const std::string& guid,
                            const std::string& description,
                            const uint64_t num_ports, database& db)
    {
        //TODO: prepared statement?
        db << "INSERT INTO hosts VALUES (?, ?, ?);"
            << guid << (int) num_ports << description;
    }


    static void create_tables(database &db)
    {
        create_host_tab(db);
        create_switch_tab(db);
        create_con_tab(db);
        create_con_details_tab(db);
    }

    static int insert_hosts(const Map_value* hosts, database& db)
    {
        int inserted = 0;
        for (const auto& host : *hosts) {
            auto host_map = host.second.get()->as<Map_value>();
            std::string guid = host_map->get<String_value>("guid")->value();
            std::string desc = host_map->get<String_value>("description")->value();
            uint64_t num_ports = host_map->get<Uint_value>("num ports")->value();
            insert_host(guid, desc, num_ports, db);
            ++inserted;
        }

        return inserted;
    }

    static int insert_switches(const Map_value* switches, database& db)
    {
        int inserted = 0;
        for (const auto& swtch : *switches) {
            auto sw_map = swtch.second.get()->as<Map_value>();
            std::string guid = sw_map->get<String_value>("guid")->value();
            std::string desc = sw_map->get<String_value>("description")->value();
            std::string port_type = sw_map->get<String_value>("port type")->value();
            uint64_t num_ports = sw_map->get<Uint_value>("num ports")->value();
            insert_switch(guid, desc, port_type, num_ports, db);
            ++inserted;
        }

        return inserted;
    }

    static int insert_connections(const Array_value* connections, database& db)
    {
        int inserted = 0;

        for (const auto& c : *connections) {
            auto con_map = c->as<Map_value>();
            uint64_t local_port = con_map->get<Uint_value>("local port")->value();
            //std::string peer_guid = con_map->get<String_value>("peer_guid")->value();
            std::string peer_guid = con_map->get<String_value>("to_guid")->value();
            std::string from_guid = con_map->get<String_value>("from_guid")->value();
            uint64_t peer_port = con_map->get<Uint_value>("peer port")->value();
            std::string peer_port_guid = con_map->get<String_value>("peer port guid")->value();
            std::string peer_desc = con_map->get<String_value>("peer description")->value();
            uint64_t peer_lid = con_map->get<Uint_value>("peer lid")->value();
            uint64_t local_lid = con_map->get<Uint_value>("local lid")->value();
            uint64_t local_lmc = con_map->get<Uint_value>("local lmc")->value();
            std::string speed = con_map->get<String_value>("speed")->value();

            insert_connection(from_guid, peer_guid, local_port, peer_port, db);
            insert_connection_details(local_port, peer_guid, peer_port, peer_port_guid,
                                        peer_desc, peer_lid, local_lid, local_lmc, speed, db);
            ++inserted;
        }

        return inserted;
    }

    Infiniband_Extractor::Infiniband_Extractor() : Extractor("Infiniband") {}


    void Infiniband_Extractor::load(Extractor_Set& findings)
    {
        auto data = collect();

        if (!data.hosts.empty()) {
            auto hosts = make_value<Map_value>();

            for (const auto& host_kvp : data.hosts) {
                auto host = host_kvp.second;
                auto h_value = make_value<Map_value>();

                h_value->add("num ports", make_value<Uint_value>(host.num_ports));
                h_value->add("guid", make_value<String_value>(host.guid));
                h_value->add("description", make_value<String_value>(host.description));

                hosts->add(host_kvp.first, std::move(h_value));
            }

            findings.add_info("hosts", std::move(hosts));
        }

        if (!data.switches.empty()) {
            auto switches = make_value<Map_value>();

            for (const auto& switch_kvp : data.switches) {
                auto sw = switch_kvp.second;
                auto sw_value = make_value<Map_value>();

                sw_value->add("num ports", make_value<Uint_value>(sw.num_ports));
                sw_value->add("guid", make_value<String_value>(sw.guid));
                sw_value->add("description", make_value<String_value>(sw.description));
                sw_value->add("port type", make_value<String_value>(sw.port_type));

                switches->add(switch_kvp.first, std::move(sw_value));
            }

            findings.add_info("switches", std::move(switches));
        }

        if (!data.connections.empty()) {
            auto connections = make_value<Array_value>();
            load_connections(data.connections, connections.get());

            findings.add_info("connections", std::move(connections));
        }
    }

    void Infiniband_Extractor::store(Extractor_Set& findings, const std::string& dbname)
    {
        database db = database(dbname);
        create_tables(db);

        // insert hosts
        auto hosts = findings.get<Map_value>("hosts");
        int inserted_hosts = insert_hosts(hosts, db);
        adafs::utils::log::logging::debug() << "database inserted: " << inserted_hosts << " hosts!";

        // insert switches
        auto switches = findings.get<Map_value>("switches");
        int inserted_switches = insert_switches(switches, db);
        adafs::utils::log::logging::debug() << "database inserted: " << inserted_switches << " switches!";

        // insert host connections
        auto conns = findings.get<Array_value>("connections");
        int inserted_connections = insert_connections(conns, db);
        adafs::utils::log::logging::debug() << "database inserted: " << inserted_connections  << " connections!";
    }

    void Infiniband_Extractor::load_connections(const std::vector<Connection>& connections, Array_value* value)
    {
        for (const auto& con : connections) {
            auto c_value = make_value<Map_value>();

            c_value->add("local port", make_value<Uint_value>(con.local_port));
            //c_value->add("peer_guid", make_value<String_value>(con.peer_guid));
            c_value->add("to_guid", make_value<String_value>(con.peer_guid));
            c_value->add("from_guid", make_value<String_value>(con.from_guid));

            c_value->add("peer port", make_value<Uint_value>(con.peer_port));
            c_value->add("peer port guid", make_value<String_value>(con.peer_port_guid));
            c_value->add("peer description", make_value<String_value>(con.peer_description));
            c_value->add("peer lid", make_value<Uint_value>(con.peer_lid));
            c_value->add("local lid", make_value<Uint_value>(con.local_lid));
            c_value->add("local lmc", make_value<Uint_value>(con.local_lmc));
            c_value->add("speed", make_value<String_value>(con.speed));

            value->add(std::move(c_value));
        }
    }

}} /* closing namespace adafs::extractor */
