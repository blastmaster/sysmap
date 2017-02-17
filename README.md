# sysmap - A tool suite for gathering static resource information.

The sysmap tools suite consists of a set of tools that provide support for gathering static resource information
from HPC systems.
The tool suite is divided into two parts:
1. The sysmap tool - A binaray tool which is deployed on your HPC Cluster to gather node local information about static resources.
2. The smap tool - A python script which can be used to process the gathered output into a database, and query them.

## Platforms

The major platform for the sysmap tool-suite are x86-linux clusters other popular architectures in the field of
HPC are also intended to be supported.

For development a linux system is suggested. Windows is not supported and support is not planned at all.

## Dependencies

* C++14 compiler
* rapidjson - required (via submodule)
* Boost - required (filesystem, program_options and system)
* Catch - optional (via submodule, just for testing)
* Nitro - required for logging (via submodule)
* sqlite_modern_cpp - required for sqlite3 (via submodule) but deprecated
* Python 3
* doxygen - required for sysmap API-documentation

## python dependecies

* click - requried (installed via setup.py)
* python-arango - required (installed via setup.py)
* graphviz - required (installed via setup.py)
* sphinx - required for Documentation
* breathe - required for documentation (sphinx-doxygen bridge)

## Build / Installation

Make sure that you have all necessary dependcies installed. Dependencies with the `via submodule` note above,
will be automatically cloned and installed via cmake during the build. All other e.g. doxygen or boost-libs must be installed.
```
cd sysmap
virtualenv --python=python3 env # create new virtualenv with pip and python3 highly recommended for development
. env/bin/activate # activate virtualenv
pip install -U sphinx breathe # install necessary dependencies
mkdir build/ ; cd build
cmake ..
make -j
```

The installation process is still work in progress, after build you will find the binaries under your build directory.
For the Python parts do the following:

**Attention:** make sure your virtualenv is active!

```
cd sysmap/python
pip install -e .
```

Thats it, propably you need to export the environment variables:
```
export LC_ALL=C.UTF-8
export LANG=C.UTF-8
```

## Arangodb

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

## License

The sysmap tool-suite is distributed under the BSD License. See the LICENSE file for the full license text.
