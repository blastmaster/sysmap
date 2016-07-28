cmake_minimum_required(VERSION 3.2.2)

project(sysmap)

# set compiler flags
set(SYSMAP_CXX_FLAGS "-Wall -Wextra -Weffc++ -std=c++14")

# find boost we use program_options, system and filesystem
find_package(Boost 1.58 REQUIRED COMPONENTS program_options system filesystem)

# add lib and bin directory
add_subdirectory(lib)
add_subdirectory(bin)
