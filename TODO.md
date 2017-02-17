# TODOS

## sysmap

* Refactor XML_Writer, the xml output is buggy and must be fixed. Take a look at rapidxml lib.

* ASCII Output, the ascii output is not really implemented yet.

* Extractor Refinement
  Especially the hwloc extractor, does something about pci, something about memory and something about cpus.
  We should split this up in more fine grained extractors which can than, also gathered by other concrete extractors if hwloc is not present.

* build system should be able to build python part too.


* Support more than one concrete extractor per abstract extractor.

## Description
  When a concrete extractor wants to registrate in the registry. It creates
  a static object of Registrar which constructor associates the name of the
  abstract extractor with a extractor_ptr to the factory function for the concrete extractor.
  Therefor we can map between abstract extractor name and the creation of a concrete extractor.
  But what, if we had more than one concrete extractor, that was possible to build for one abstract extractor.
  If each of these concrete extractors try to registrate with the same name of the abstract extractor,
  then we get a collision in the key of the map.

## Problem
  Through the collision in the registry map which is of type std::unordered_map<std::string, extractor_ptr>.
  We override the before registered entry in the registry. Additionally since the order of static initialization
  is undefined in cpp. We lose control, which concrete extractor would actually be instantiated.

## Solutions
  One possible Solution would be to use an std::unordered_multimap<std::string, extractor_ptr> as a type for the registry_map.
  Than multiple keys can be inserted. We could use the equal_range() to iterate over all possible concrete extractors and
  call a static check() function which indicates whether the concrete extractor are able to run or not.
  Choose the first concrete extractor which is able to run for instatiation.
  One advantage of that approach could be, that we can check for files, access rights, programs, etc... .
  In a much better way as we could at build-time with the help of the buildsystem.

  Another solution could be that each concrete extractor registrates with his full name.
  This would cause some more typing effort for the user.
  But is much less work for me :).

* Support for configuration file
  Here, we could also solve the problem of which concrete extractor should be used by explicitly name them.
  Define how the programm should run.

* Custom knowledge insertion.
  Support the insertion of user defined knowledge about the system.

## smap

aql query nodes/<hostname>
