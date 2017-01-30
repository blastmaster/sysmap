import click
import json

import pprint

from sysmap.database import SystemMapDatabase
from sysmap.graph import NetworkGraph

__author__ = "Sebastian Oeste"
__email__ = "sebastian.oeste@tu-dresden.de"


QUERY_FACTS = [
    'disks',
    'mountpoints',
    'partitions',
    'pcidevices',
    'machineinfo',
    'bandwidth',
    'infiniband',
]

OUTPUT_FORMATS = [
    'ascii',
    'json',
]


class FactNotFoundError(Exception):
    ''' Exception that is raised, when a query fact could not found in a received Dataset. '''
    pass


class Node:
    ''' Representation of a Node, contains all the node-local data extracted there from. '''

    _data = {}

    def __init__(self, hostname, node_data=None):
        self.hostname = hostname
        node_data.update({'_key': self.hostname})
        self._data = node_data

    def __repr__(self):
        return '<Node {}>'.format(self.hostname)

    def __getitem__(self, key):
        return self._data[key]

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
@click.argument('files', nargs=-1, type=click.File('r'))
@pass_db
def insert(sysdb, files, network):
    ''' Insert extracted data into database. '''

    for filename in files:
        data = json.load(filename)
        hostname = data['MachineInfo']['Hostname']
        if network:
            netgraph = NetworkGraph()
            insert_network_connections(netgraph, sysdb)
        node = Node(hostname, data)
        insert_node(node.data, sysdb)


@cli.command(help='The query that should be applied to the active set.')
@click.option('-q', '--query-facts', multiple=True,
              type=click.Choice(QUERY_FACTS),
              help='Facts of nodes that should be queried from the database.')
@click.option('-f', '--format', 'fmt', type=click.Choice(OUTPUT_FORMATS),
              default='ascii', help='Output format')
@click.argument('nodes', nargs=-1)
@pass_db
def query(sysdb, query_facts, fmt, nodes):
    ''' Query facts from the system database. '''

    pprint.pprint(sysdb.current_database)
    pprint.pprint(query_facts)
    pprint.pprint(nodes)

    for node in get_nodeset(sysdb, nodes):
        try:
            for result in do_query(node, query_facts):
                print_result(result, fmt)
        except FactNotFoundError as err:
            click.echo(click.style('Warning: query fact: {} could not be found!'.format(err),
                                   fg='red', bold=True))


def get_nodeset(sysdb, nodes):
    ''' Return a set of node documents form the database specified by :param: nodes. '''

    cursor = sysdb.execute_query('for node in nodes filter @value ANY == node._key return node',
                                 bind_vars={'value': nodes})
    for c in cursor:
        yield Node(c['_key'], c)


def do_query(node, facts):

    for fact in facts:
        try:
            yield node[fact]
        except KeyError:
            raise FactNotFoundError(fact)


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


def print_result(result, fmt):
    ''' Calls the appropriate function for the given format to print the result.

    :param result: Result data from the query.
    :type result: dict
    :param fmt: The format in which the result should printed.
    :type fmt: str
    '''
    if fmt == 'ascii':
        print_result_ascii(result)
    elif fmt == 'json':
        raise NotImplementedError('Output format json')


def print_result_ascii(result):
    ''' Print result as ascii.

    :param result: Result data from the query.
    :type result: dict
    '''
    pprint.pprint(result)
