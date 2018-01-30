cmake_minimum_required(VERSION 3.2.2)

set(LIBSYSMAP_VERSION_MAJOR 0)
set(LIBSYSMAP_VERSION_MINOR 1)
set(LIBSYSMAP_VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS ${SYSMAP_CXX_FLAGS})

#TODO add just platfrom independent sources here
set(LIBSYSMAP_SOURCES
    "src/extractors/filesystem_extractor.cc"
    "src/extractors/disk_extractor.cc"
    "src/extractors/infiniband_extractor.cc"
    "src/extractors/kernel_extractor.cc"
    "src/extractors/pci_device_extractor.cc"
    "src/extractors/memory_info_extractor.cc"
    "src/extractors/machine_info_extractor.cc"
    "src/linux/filesystem_extractor.cc"
    "src/linux/disk_extractor.cc"
    "src/linux/infiniband_extractor.cc"
    "src/linux/kernel_extractor.cc"
    "src/hwloc/hwloc_machine_info_extractor.cc"
    "src/hwloc/hwloc_pci_device_extractor.cc"
    "src/hwloc/hwloc_memory_info_extractor.cc"
    "src/utils/file.cc"
    "src/utils/execution.cc"
    "src/array_value.cc"
    "src/map_value.cc"
    "src/scalar_value.cc"
    "src/extractor.cc"
    "src/extractor_set.cc"
    "src/output.cc"
)

include_directories(
    include
    ${RAPIDJSON_INCLUDE_DIR}
    ${SQLITE3_INCLUDE_DIR}
    ${SQLITE_MODERN_CPP_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${HWLOC_INCLUDE_DIRS}
    ${PUGIXML_INCLUDE_DIR}
    ${YAML_CPP_SOURCE_DIR}/include
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${HWLOC_LIBRARIES}
)

# build static or shared library
if(BUILD_STATIC)
    add_library(libsysmap STATIC ${LIBSYSMAP_SOURCES})
else()
    add_library(libsysmap SHARED ${LIBSYSMAP_SOURCES})
endif()

target_link_libraries(libsysmap PRIVATE
    ${Boost_LIBRARIES}
    ${HWLOC_LIBRARIES}
    yaml-cpp
)

# install library in lib/
install(TARGETS libsysmap DESTINATION lib)

# install the header files in include/
install(DIRECTORY include DESTINATION include)
