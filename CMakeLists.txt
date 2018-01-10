cmake_minimum_required(VERSION 3.2.2)

set(PROJECT_NAME "sysmap")
set(PROJECT_VERSION 0.0.1)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

include(CTest)

# Build static or shared objects
option(BUILD_STATIC "link static if this is off a shared lib will be build" ON)

# TODO add option for running tests
option(RUN_TESTS "run test suite" ON)

# Build documentation
option(BUILD_DOC "Build the documentation" ON)

# Debbuging disables BUILD_DOC
option(DEBUG_MODE "Provides faster compilation for debugging purpose" OFF)


# set compiler flags
set(SYSMAP_CXX_FLAGS "-Wextra -std=c++14 -D PUGIXML_HEADER_ONLY")
# cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# DEBUG
message("TOP LEVEL CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# Find dependencies
# find boost we use program_options, system and filesystem
find_package(Boost 1.58 REQUIRED COMPONENTS program_options system filesystem iostreams)
# find hwloc
find_package(Hwloc REQUIRED)

add_subdirectory(external)
# include nitro lib
include(external/nitro/Nitro.cmake)

# setting the doxygen input directories to:
# sysmap/lib/include
# sysmap/lib/src
# sysmap/bin


# Handling Options
if(RUN_TESTS)
    enable_testing()
    add_subdirectory(test)
else()
    message("ATTENTION: RUN_TESTS Disabled, no Test will be build")
endif()

if(DEBUG_MODE)
    message("DEBUG_MODE ENABLED")
    set(SYSMAP_CXX_FLAGS "${SYSMAP_CXX_FLAGS} -ggdb -Wall")
    message("ATTENTION: no Documentation will be build")
endif()

if(BUILD_DOC AND NOT DEBUG_MODE)
    # find sphinx for documentation
    find_package(Sphinx REQUIRED)
    # find doxygen for API-documentation
    find_package(Doxygen REQUIRED)
    list(APPEND DOXY_INPUT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/lib/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/src ${CMAKE_CURRENT_SOURCE_DIR}/bin")
    message("TOP LEVEL DOXY_INPUT_FILES: ${DOXY_INPUT_FILES}")
    add_subdirectory(doc)
endif()

# add lib, bin, doc and test directory
add_subdirectory(lib)
add_subdirectory(bin)
