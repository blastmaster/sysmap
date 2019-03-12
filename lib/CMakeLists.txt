cmake_minimum_required(VERSION 3.10.1)

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
    "src/extractors/network_device_extractor.cc"
    "src/linux/filesystem_extractor.cc"
    "src/linux/disk_extractor.cc"
    "src/linux/infiniband_extractor.cc"
    "src/linux/kernel_extractor.cc"
    "src/linux/network_device_extractor.cc"
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

# build static or shared library
if(BUILD_STATIC)
    add_library(libsysmap STATIC ${LIBSYSMAP_SOURCES})
else()
    add_library(libsysmap SHARED ${LIBSYSMAP_SOURCES})
endif()

add_dependencies(libsysmap yaml-cpp-project)
add_dependencies(libsysmap nitro-project)
add_dependencies(libsysmap rapidjson-project)
add_dependencies(libsysmap pugixml-project)
add_dependencies(libsysmap sqlite_modern_cpp-project)

target_include_directories(libsysmap
    PUBLIC
        include
    PRIVATE
        ${SQLITE3_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${HWLOC_INCLUDE_DIRS}
        $<TARGET_PROPERTY:RapidJSON::RapidJSON,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:SQLITE_MODERN_CPP::SQLITE_MODERN_CPP,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:YAML::YAML,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Nitro::Nitro,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:PugiXML::PugiXML,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(libsysmap
    PRIVATE
        ${Boost_LIBRARIES}
        ${HWLOC_LIBRARIES}
        YAML::YAML

)

target_compile_options(libsysmap
    PUBLIC
        "-Wextra"
        "-std=c++14"
        "-D PUGIXML_HEADER_ONLY"
)

# install library in lib/
install(TARGETS libsysmap DESTINATION lib)

# install the header files in include/
install(DIRECTORY include DESTINATION include)
