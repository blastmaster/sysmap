import click
import json

from sysmap.database import SystemMapDatabase
from sysmap.graph import NetworkGraph

import pprint

__author__ = "Sebastian Oeste"
__email__ = "sebastian.oeste@tu-dresden.de"

QUERY_FACTS = [
        'filesystem',
        'storage',
        'bandwidth',
        'infiniband',
]


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


pass_db = click.make_pass_decorator(SystemMapDatabase)

@click.group()
@click.option('-c', '--conf', #type=click.File('r'),
              help="Configuration file.")
@click.option('--database', help='Name of the database that should be used.')
@click.pass_context
def cli(ctx, conf, database):
    ''' smap query/insert/update the system-map database. '''

    ctx.obj = SystemMapDatabase(database, conf)


@cli.command()
@click.option('--network/--no-network', default=False,
              help='should the network be extracted')
@click.argument('filename', type=click.File('r'))
@pass_db
def insert(sysdb, filename, network):
    ''' Insert extracted data into database. '''

    data = json.load(filename)
    hostname = data['MachineInfo']['Hostname']
    if network:
        netgraph = NetworkGraph()
        insert_network_connections(netgraph, sysdb)
    node = Node(hostname, data)
    insert_node(node.data, sysdb)


@cli.command(help='The query that should be applied to the active set.')
@click.option('--query-facts', type=click.Choice(QUERY_FACTS),
              help='Facts of nodes that should be queried from the database.')
@click.argument('nodes', nargs=-1)
@pass_db
def query(sysdb, query_facts, nodes):
    ''' Query facts from the system database. '''

    pprint.pprint(sysdb.current_database)
    pprint.pprint(query_facts)
    pprint.pprint(nodes)

    cursor = sysdb.execute_query('for node in nodes return node.MachineInfo.Hostname')
    for c in cursor:
        print('recieved: {}'.format(c))


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
