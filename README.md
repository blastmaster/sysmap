# sysmap - A tool suite for gathering static resource information.

The sysmap tools suite consists of a set of tools that provide support for gathering static resource information
from HPC systems.
The tool suite is divided into two parts:
1. The sysmap tool - A binaray tool which is deployed on your HPC Cluster to gather node local information about static resources.
2. The smap tool - A python script which can be used to process the gathered output into a database, and query them.

## Dependencies

* C++14 compiler
* rapidjson - required (via submodule)
* Boost - required (filesystem, program_options)
* Catch - optional (via submodule, just for testing)
* Nitro - required for logging (via submodule)
* sqlite_modern_cpp - required for sqlite3 (via submodule) but deprecated
* Python 3
* doxygen - required for sysmap API-documentation

## python dependecies

* click - requried (installed via setup.py)
* python-arango - required (installed via setup.py)
* sphinx - required for Documentation
* breathe - required for documentation (sphinx-doxygen bridge)

### Arangodb

ArangoDB is a multi-model NoSQL Database, supporting key-value, documents and graph storage.
We use ArangoDB as our central Database for collected system information.

For download and installation instructions for your distribution see the [arangodb webite](https://www.arangodb.com/download-major/).


### sqlite3 (not used, not maintained, still in source, maybe deprecated)

table node
    hostname string not null primary key
    guid string // to identify hosts entry


hwloc extractor

table machine_info
    hostname string not null primary key
    [...]

table pci_device
    [...]

table memory_info
    [...]


filesystem extractor

table mountpoint
    [...]

table partition
    [...]


disk extractor

table disk
    device_number string not null primary key
    [...]


infiniband extractor (already implemented)

table connections
    from_guid string not null primary key
    to_guid string not null primary key
    from_port int not null primary key
    to_port int not null primary key

table connection_details
    [...]

table hosts
    guid string not null primary key
    [...]

table switch
    guid string not null primary key
    [...]
