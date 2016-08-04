cmake_minimum_required(VERSION 3.2.2)

set(LIBSYSMAP_VERSION_MAJOR 0)
set(LIBSYSMAP_VERSION_MINOR 1)
set(LIBSYSMAP_VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS ${SYSMAP_CXX_FLAGS})

#TODO add just platfrom independent sources here
set(LIBSYSMAP_SOURCES
    "src/extractors/filesystem_extractor.cc"
    "src/extractors/disk_extractor.cc"
    "src/extractors/hwloc_extractor.cc"
    "src/linux/filesystem_extractor.cc"
    "src/linux/disk_extractor.cc"
    "src/hwloc/hwloc_extractor.cc"
    "src/utils.cc"
    "src/array_value.cc"
    "src/map_value.cc"
    "src/extractor_set.cc"
    "src/output.cc"
)


include_directories(
    include
    ${Boost_INCLUDE_DIRS}
    ${HWLOC_INCLUDE_DIRS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${HWLOC_LIBRARIES}
)

# build static library
add_library(libsysmap STATIC ${LIBSYSMAP_SOURCES})

target_link_libraries(libsysmap PRIVATE
    ${Boost_LIBRARIES}
    ${HWLOC_LIBRARIES}
)

# install library in lib/
install(TARGETS libsysmap DESTINATION lib)

# install the header files in include/
install(DIRECTORY include DESTINATION include)
