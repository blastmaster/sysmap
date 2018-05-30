# sysmap - A tool suite to gather static resource information.

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
* hwloc - required
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
pip install -U "sphinx==1.5.6" breathe # install necessary dependencies (building doc is failing with newer releases than 1.5.6)
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
## sysquery - A tool for querying the sysmap database

**Usage**

###### Print help message:
```
    sysquery -h
```

###### Query everything:
```
    sysquery -f path/to/db/file
```
This returns a JSON String containing all Hosts and their Datastrings of each extractor

###### Query specific Host:
```
    sysquery -f path/to/db/file -n hostname
```

###### Query specific Extractor:
```
    sysquery -f path/to/db/file -e disks
```

###### Querying specific Hosts and Extractors:
```
    sysquery -f path/to/db/file -n hostname1 hostname2 hostname 3 -e disks pcidevices
```
This returns the Datastrings of the given Extractors for the given Hosts

###### Listing available Hosts:
```
    sysquery -f path/to/db/file -l
```

###### Listing available Extractors:
```
    sysquery -f path/to/db/file -L
```

###### Specifying Output file:
```
    sysquery -f path/to/db/file -o path/to/file
```
This will write the output to the given file instead of std::cout


## License

The sysmap tool-suite is distributed under the BSD License. See the LICENSE file for the full license text.
