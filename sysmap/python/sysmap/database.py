import arango
import configparser

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

    def execute_query(self, query_str, **kwargs):

        self.database.aql.validate(query_str)
        return self.database.aql.execute(query_str, **kwargs)

