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


class NetworkGraph:
    ''' Representation of the network connections.
        This class contains the entities in the network e.g. hosts or switches.
        And the connections between them.
    '''

    Vertex = namedtuple('Vertexes', ['hosts', 'switches'])
    _edges = []

    def _sanitize_edge_connection(self, edge):
        ''' Update '_to' and  '_form' field of a edge.

        :param edge: One edge connection.
        :type edge: dict
        :returns: Updated edge with _to and _from key.
        :rtype: dict
        '''
        if edge['to_guid'].startswith('S'):
            to_collection = 'switches/'
        elif edge['to_guid'].startswith('H'):
            to_collection = 'hosts/'

        if edge['from_guid'].startswith('S'):
            from_collection = 'switches/'
        elif edge['from_guid'].startswith('H'):
            from_collection = 'hosts/'

        edge.update({
            '_to': to_collection + edge['to_guid'],
            '_from': from_collection + edge['from_guid']
        })

        return edge

    def _sanitize_vertexes(self, vertex):
        ''' Update '_key' field of vertex to appropriate guid.

        :param vertex: Vertex
        :type vertex: dict
        :returns: An updated dict, '_key' field with 'guid' value.
        :rtype: dict
        '''
        vertex.update({'_key': vertex['guid']})
        return vertex

    def __init__(self, hsts=None, switches=None, connections=None):
        self._vertexes = self.Vertex(hosts=[self._sanitize_vertexes(h) for h in hsts],
                                    switches=[self._sanitize_vertexes(s) for s in switches])
        self._edges = [self._sanitize_edge_connection(c) for c in connections]

    @property
    def vertexes(self):
        ''' Returns a concatenated list of all vertexes.

        :returns: List of vertexes, contains of hosts and switches.
        :rtype: List[dict]
        '''
        return self._vertexes.hosts + self._vertexes.switches

    @property
    def switches(self):
        ''' Returns a list of all 'switch' vertexes.

        :returns: List of all switches.
        :rtype: List[dict]
        '''
        return self._vertexes.switches

    @property
    def hosts(self):
        ''' Returns a list of all 'host' vertexes.

        :returns: List of all hosts.
        :rtype: List[dict]
        '''
        return self._vertexes.hosts

    @property
    def edges(self):
        ''' Return a list of all 'connection' edges.

        :returns: List of all connections.
        :rtype: List[dict]
        '''
        return self._edges

    def to_graph(self, graphargs):
        ''' Draw a dot graph of the network graph.

        :params graphargs: Arguments to graphviz.Digraph.
        :type graphargs: dict
        '''
        graph = Digraph(**graphargs)
        for v in self._vertexes:
            graph.node(v['guid'], v['description'])
        for c in self._edges:
            graph.edge(c['from_guid'], c['to_guid'])

        graph.render()


class Node:
    ''' Representation of a Node, contains all the node-local data extracted there from. '''

    _data = {}

    def __init__(self, hostname, node_data=None):
        self.hostname = hostname
        node_data.update({'_key': self.hostname})
        self._data = node_data

    @property
    def data(self):
        return self._data

    @property
    def name(self):
        return self.hostname


def insert_node(nodedata, systemdb, collection_name='nodes'):
    ''' Insert node-local data of one node into the system-map database.

    :param nodedata: Node-local data to insert into collection.
    :type nodedata: dict
    :param systemdb: System-Map-Database-Object
    :type systemdb: SystemMapDatabase
    :param collection_name: Name of the collection to insert into.
    :type collection_name: str
    '''
    return systemdb.insert_collection(collection_name, nodedata)

def insert_network_connections(ngraph, systemdb):
    ''' Insert a whole NetworkGraph into the system-map database.

    :param ngraph: Network graph to be inserted.
    :type ngraph: NetworkGraph
    :param systemdb: SystemMapDatabase Object
    :type systemdb: SystemMapDatabase
    '''
    # TODO: prevent double insertions
    systemdb.insert_vertex('networkgraph', 'hosts', ngraph.hosts)
    systemdb.insert_vertex('networkgraph', 'switches', ngraph.switches)
    systemdb.insert_edges('networkgraph', 'edges', ['hosts', 'switches'], ['switches', 'hosts'], ngraph.edges)


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
