#include "value.hpp"
#include "scalar_value.hpp"
#include "map_value.hpp"
#include "array_value.hpp"
#include "utils.hpp"

#include "extractors/infiniband_extractor.hpp"

namespace adafs { namespace extractor {

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
