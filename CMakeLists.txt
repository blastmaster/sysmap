cmake_minimum_required(VERSION 3.2.2)

set(PROJECT_NAME "sysmap")
set(PROJECT_VERSION 0.0.1)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# Build static or shared objects
option(BUILD_STATIC "link static if this is off a shared lib will be build" ON)

# TODO add option for running tests
option(RUN_TESTS "run test suite" ON)

# Build documentation
option(BUILD_DOC "Build the documentation" ON)

# set compiler flags
set(SYSMAP_CXX_FLAGS "-Wall -Wextra -std=c++14 -ggdb")
# cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# DEBUG
message("TOP LEVEL CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# Find dependencies
# find boost we use program_options, system and filesystem
find_package(Boost 1.58 REQUIRED COMPONENTS program_options system filesystem)
# find hwloc
find_package(Hwloc REQUIRED)
# find sqlite3
find_package(sqlite3 REQUIRED)
# find sphinx for documentation
find_package(Sphinx REQUIRED)
# find doxygen for API-documentation
find_package(Doxygen REQUIRED)

add_subdirectory(external)
# include nitro lib
include(external/nitro/Nitro.cmake)

# setting the doxygen input directories to:
# sysmap/lib/include
# sysmap/lib/src
# sysmap/bin
list(APPEND DOXY_INPUT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/src ${CMAKE_CURRENT_SOURCE_DIR}/bin")
message("TOP LEVEL DOXY_INPUT_FILES: ${DOXY_INPUT_FILES}")

# add lib, bin, doc and test directory
add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(doc)
add_subdirectory(test)
