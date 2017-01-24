from graphviz import Digraph
from collections import namedtuple


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
