#ifndef __ADAFS_EXTRACTOR_INFINIBAND_EXTRACTOR_HPP__
#define __ADAFS_EXTRACTOR_INFINIBAND_EXTRACTOR_HPP__

#include "../extractor_set.hpp"
#include "../extractor.hpp"

#include "array_value.hpp"


namespace adafs { namespace extractor {

struct Infiniband_Extractor : Extractor {

    Infiniband_Extractor();

    virtual void load(Extractor_Set& findings) override;
    virtual void store(Extractor_Set& findings) override;

protected:

    enum class Peer_Type {
        Switch,
        Host,
    };

    struct Connection {
        Peer_Type type; // type of peer {switch,host}
        uint64_t local_port; // local port number
        std::string peer_guid; // guid of peer
        uint64_t peer_port; // port of peer
        std::string peer_port_guid; // port guid of peer if peer is host
        std::string peer_description; // node description of peer
        uint64_t peer_lid; // lid of peer
        uint64_t local_lid; // local lid
        uint64_t local_lmc; // local lmc
        std::string speed; // speed
    };

    struct Switch {
        uint64_t num_ports; // number of ports
        std::string guid;   // guid
        std::string description; // node description
        std::string port_type;     // type of port 0 {base,enhanced}
        std::vector<Connection> connections;
    };

    struct Host {
        uint64_t num_ports; // number of ports
        std::string guid;   // guid
        std::string description; // node description
        std::vector<Connection> connections;
    };

    struct data {
        std::map<std::string, Host> hosts; // map of hosts with guid as key and host as value
        std::map<std::string, Switch> switches; // map of switches with guid as key and switch as value
    };

    virtual data collect() = 0;

private:

    void load_connections(const std::vector<Connection>& connections, Array_value* value);

};

}} /* closing namespace adafs::extractor */

#endif /* __ADAFS_EXTRACTOR_INFINIBAND_EXTRACTOR_HPP__ */
