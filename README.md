# sysmap

### Dependencies

* rapidjson
* Catch
* Nitro

## python

### Dependencies

* click
* python-arango
* sphinx
* breathe

### Arangodb


### sqlite3 (not used)

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

