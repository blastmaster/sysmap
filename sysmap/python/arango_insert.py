#! /usr/bin/env python

import sys
import argparse
from graphviz import Digraph
from collections import namedtuple

from sysmap.database import SystemMapDatabase

import json
import pprint

__author__ = "Sebastian Oeste"
__email__ = "sebastian.oeste@tu-dresden.de"

'''
    This script inserts the collected data from the `sysmap` tool into arangodb.
    A HPC-System is a Database instance conaining typically a nodes collection,
    here all the data from the node-local extractors are stored.
    If the network topology is even present, an extra hosts, and switches
    collection will be created and a edge_collection which stores the connections between switches and hosts.
'''


def parse_arguments():
    ''' This utility function parses the arguments from 'sys.argv'.

    :returns: parsed arguments
    :rtype: argparse.Namespace
    '''

    parser = argparse.ArgumentParser(description="Insert sysmap output into arangodb.")
    parser.add_argument('file', type=str, help="input jsonfile, the output from the 'sysmap' tool.")
    parser.add_argument('-c', '--conf',  type=str, help="Configuration file for the database connection and authentification.")
    parser.add_argument('--database', type=str, help="The name of the database to insert into.")
    parser.add_argument('--network', action='store_true', help="Insert network-topology-graph into database.")
    # database name
    # extractor names?

    return parser.parse_args()

def main():
    '''
        clustername: should be the name of the database, if not already existing, create new one.
        e.g. taurus.
    '''

    args = parse_arguments()
    sysdb = SystemMapDatabase(args.database, args.conf)

    with open(args.file) as jsonfile:
        data = json.load(jsonfile)
        hostname = data['MachineInfo']['Hostname']
        if args.network:
            ng = NetworkGraph(data.pop('hosts').values(),
                              data.pop('switches').values(),
                              data.pop('connections'))
            insert_network_connections(ng, sysdb)
        n = Node(hostname, data)
        insert_node(n.data, sysdb)

main()
