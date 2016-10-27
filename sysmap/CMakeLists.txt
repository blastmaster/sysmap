cmake_minimum_required(VERSION 3.2.2)

project(sysmap)

# set compiler flags
set(SYSMAP_CXX_FLAGS "-Wall -Wextra -Weffc++ -std=c++14 -ggdb")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(nitro/Nitro.cmake)

# find boost we use program_options, system and filesystem
find_package(Boost 1.58 REQUIRED COMPONENTS program_options system filesystem)
# find hwloc
find_package(Hwloc REQUIRED)

# find sqlite3
find_package(sqlite3 REQUIRED)
# add lib and bin directory
add_subdirectory(lib)
add_subdirectory(bin)
