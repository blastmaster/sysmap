#include "linux/infiniband_extractor.hpp"
#include "utils.hpp"


namespace adafs { namespace linux {

    Infiniband_Extractor::data Infiniband_Extractor::collect()
    {
        data result;
        collect_ibnetdiscover(result);
        return result;
    }

    void Infiniband_Extractor::collect_ibnetdiscover(data& result)
    {
        std::string connection_guid;
        utils::file::for_each_line("../test_data/ib_data/ibnetdiscover_plain_taurusi6593", [&](const std::string& line) {
                // define pattern for switch line
                std::regex switch_pattern(R"(^Switch\s+(\d+)\s+\"([\w-]+)\"\s+#\s+\"([^\"]+)\"\s+(\w+)(.*)$)");
                // define pattern for switch connectivity line
                std::regex switch_connectivity_pattern(R"(^\[(\d+)\]\s+\"([\w-]+)\"\[(\d+)\](?:\(([^\)]+)\))?\s+#\s+\"([^\"]+)\"\s+lid\s+(\d+)\s+(.*)$)");
                // define pattern for host line
                std::regex host_pattern(R"(^Ca\s+(\d+)\s+\"([\w-]+)\"\s+#\s+\"([^\"]+)\"$)");
                // define pattern for host connectivity line
                std::regex host_connectivity_pattern(R"(^\[(\d+)\]\(([^\)]+)\)\s+\"([^\"]+)\"\[(\d+)\]\s+#\s+lid\s+(\d+)\s+lmc\s+(\d+)\s+\"([^\"]+)\"\s+lid\s+(\d+)\s+(.*)$)");

                std::smatch matches;
                if (std::regex_search(line, matches, switch_pattern)) {
                    connection_guid = assemble_switch(matches, result);
                    return true;
                }
                else if (std::regex_search(line, matches, switch_connectivity_pattern)) {
                    assemble_connection(matches, result, Peer_Type::Switch, connection_guid);
                    return true;
                }
                else if (std::regex_search(line, matches, host_pattern)) {
                    connection_guid = assemble_host(matches, result);
                    return true;

                }
                else if (std::regex_search(line, matches, host_connectivity_pattern)) {
                    assemble_connection(matches, result, Peer_Type::Host, connection_guid);
                    return true;
                }

                return true;
            });
    }

    std::string Infiniband_Extractor::assemble_switch(const std::smatch& matches, data& result_graph)
    {
        // define temporary switch
        Switch sw;

        // fill temporary except connections
        //num_ports
        auto numports_s = utils::file::trim_regex_match(matches[1]);
        sw.num_ports = std::stoi(numports_s);
        // guid
        auto guid_s = utils::file::trim_regex_match(matches[2]);
        sw.guid = guid_s;
        // description
        sw.description = utils::file::trim_regex_match(matches[3]);
        // port type
        auto port_type_s = utils::file::trim_regex_match(matches[4]);
        sw.port_type = port_type_s;

        // copy temporary switch in result graph
        result_graph.switches.emplace(std::make_pair(guid_s, sw));
        return guid_s;
    }

    std::string Infiniband_Extractor::assemble_host(const std::smatch& matches, data& result_graph)
    {
        // define temporary host
        Host host;

        // fill temporary except connections
        //num_ports
        auto numports_s = utils::file::trim_regex_match(matches[1]);
        host.num_ports = std::stoi(numports_s);
        // guids
        auto guid_s = utils::file::trim_regex_match(matches[2]);
        host.guid = guid_s;
        // description
        host.description = utils::file::trim_regex_match(matches[3]);

        // copy temporary host in result graph
        result_graph.hosts.emplace(std::make_pair(guid_s, host));
        return guid_s;
    }

    void Infiniband_Extractor::assemble_connection(const std::smatch& matches, data& result_graph, const Peer_Type type, const std::string& from_guid)
    {
        // define temporary connection object
        Connection con;
        con.type = type;

        if (con.type == Peer_Type::Switch) {
            // local port
            auto local_port_s = utils::file::trim_regex_match(matches[1]);
            con.local_port = std::stoi(local_port_s);
            // peer guid
            con.peer_guid = utils::file::trim_regex_match(matches[2]);
            // peer port
            auto peer_port_s = utils::file::trim_regex_match(matches[3]);
            con.peer_port = std::stoi(peer_port_s);
            // port_guid
            con.peer_port_guid = utils::file::trim_regex_match(matches[4]);
            // peer description
            con.peer_description = utils::file::trim_regex_match(matches[5]);
            // peer lid
            auto peer_lid_s = utils::file::trim_regex_match(matches[6]);
            con.peer_lid = std::stoi(peer_lid_s);
            // speed
            con.speed = utils::file::trim_regex_match(matches[7]);

            // push back connection to from_guid switch connections vector
            result_graph.switches[from_guid].connections.push_back(con);
        }
        else if (con.type == Peer_Type::Host) {
            // local port
            auto local_port_s = utils::file::trim_regex_match(matches[1]);
            con.local_port = std::stoi(local_port_s);
            // port guid
            con.peer_port_guid = utils::file::trim_regex_match(matches[2]);
            // peer guid
            con.peer_guid = utils::file::trim_regex_match(matches[3]);
            // peer port
            auto peer_port_s = utils::file::trim_regex_match(matches[4]);
            con.peer_port = std::stoi(peer_port_s);
            // local lid
            auto local_lid_s = utils::file::trim_regex_match(matches[5]);
            con.local_lid = std::stoi(local_lid_s);
            // local lmc
            auto local_lmc_s = utils::file::trim_regex_match(matches[6]);
            con.local_lmc = std::stoi(local_lmc_s);
            // peer description
            con.peer_description = utils::file::trim_regex_match(matches[7]);
            // peer lid
            auto peer_lid_s = utils::file::trim_regex_match(matches[8]);
            con.peer_lid = std::stoi(peer_lid_s);
            // speed
            con.speed = utils::file::trim_regex_match(matches[9]);

            // push bach connection to from_guid host connections vector.
            result_graph.hosts[from_guid].connections.push_back(con);
        }
        else {
            adafs::utils::log::logging::error() << "[Error] Infiniband_Extractor::assemble_connection invalid type.\n";
            return;
        }
    }

}} /* closing namespace adafs::linux */
