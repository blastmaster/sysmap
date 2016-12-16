#! /usr/bin/env python

import sys
import os
import argparse
import configparser
import arango
from graphviz import Digraph
from collections import namedtuple

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


class SystemMapDatabase:

    _default_datbase_options = {'protocol': 'http',
                                'host': 'localhost',
                                'port': 8529,
                                'enable_logging': True}

    def _read_conf(self, conffile=None):
        ''' Read the Database configuration.

        :param conffile: The database configuration file.
        :type conffile: str | unicode
        :returns: The configuration.
        :rtype: configpargser.ConfigParser
        '''

        parser = configparser.ConfigParser(self._default_datbase_options)
        parser.read(conffile)
        return parser

    def __init__(self, dbname=None, configfile=None):
        self._conf = self._read_conf(configfile)
        client_options = dict(self._conf['Arangodb'])
        self.client = arango.ArangoClient(**client_options)
        self.database_name = self._conf.get('Database', 'name', fallback=dbname)
        if self.database_name not in self.client.databases():
            self.database = self.client.create_database(self.database_name)
        else:
            self.database = self.client.db(self.database_name)

    @property
    def current_database(self):
        ''' Currently used database name.

        :returns: database name
        :rtype: str
        '''

        return self.database_name

    def databases(self):
        ''' Returns available Databases.
        :returns: All available database names.
        :rtype: List[str]
        '''

        return self.client.databases()

    def get_graph(self, graphname):
        ''' Getting a graph object from the Database.
        If the requested graph object does not exists, a new one is created.

        :param graphname: Name of the Graph.
        :type graphname: str
        :returns: the requested graph object
        '''
        if graphname not in [g['name'] for g in self.database.graphs()]:
            return self.database.create_graph(graphname)
        return self.database.graph(graphname)

    def get_collection(self, collectionname):
        ''' Getting a collection object from the Database.
        If the requested collection does not exists, a new one is created.

        :param collectionname: Name of the collection
        :type collectionname: str
        :returns: the requested collection
        '''
        if collectionname not in [c['name'] for c in self.database.collections()]:
            return self.database.create_collection(collectionname)
        return self.database.collection(collectionname)

    def get_vertex_collection(self, collectionname, graph):
        ''' Getting a vertex collection from the Database.
        If the requested vertex collection does not exists, a new one is
        created.

        :param collectionname: Name of the collection
        :type collectionname: str
        :param graph: Graph object where the vertex collection should consists to.
        :type graph: graph object
        :returns: vertex collection object
        '''
        try:
            v_coll = graph.create_vertex_collection(collectionname)
        except arango.exceptions.VertexCollectionCreateError:
            v_coll = graph.vertex_collection(collectionname)

        return v_coll

    def insert_vertex(self, graphname, v_collection, data):
        ''' Insert vertex data into graph.

        :param graphname: Name of the Graph insert into.
        :type graphname: str
        :param v_collection: Name of the vertex collection of the Graph.
        :type v_collection: str
        :param data: Vertex data, which should be inserted.
        :type data: dict
        :returns: List of all inserted document key/revisions.
        :rtype: list
        '''
        graph = self.get_graph(graphname)
        v_coll = self.get_vertex_collection(v_collection, graph)
        return v_coll.insert(data)

    def insert_edges(self, graphname, e_collection, from_coll, to_coll, data):
        ''' Insert edge collection into graph.

        :param graphname: Name of the graph insert into.
        :type graphname: str
        :param e_collection: Name of the edge collection of the Graph.
        :type e_collection: str
        :param from_coll: Definition where the edge connection starts.
        :type from_coll: string | list of strings
        :param to_coll: Definition wher the edge connection ends.
        :type to_coll: string | list of strings
        :param data: Edge data, which should be inserted.
        :type data: dict
        '''
        graph = self.get_graph(graphname)
        try:
            e_coll = graph.create_edge_definition(e_collection,
                                                from_collections=from_coll,
                                                to_collections=to_coll)
        except arango.exceptions.EdgeDefinitionCreateError:
            e_coll = graph.edge_collection(e_collection)

        return [e_coll.insert(e) for e in data]

    def insert_collection(self, collectionname, data):
        ''' Returns the result of the insert (e.g. document key, revision)

        :param collectionname: Name of collection insert into.
        :type collectionname: str
        :param data: collection data to insert.
        :type data: dict
        '''
        collection = self.get_collection(collectionname)
        return collection.insert(data)


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
