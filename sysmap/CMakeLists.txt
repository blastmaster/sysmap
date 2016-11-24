cmake_minimum_required(VERSION 3.2.2)

set(PROJECT_NAME "sysmap")
set(PROJECT_VERSION 0.0.1)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# set compiler flags
#set(SYSMAP_CXX_FLAGS "-Wall -Wextra -Weffc++ -std=c++14 -ggdb")
set(SYSMAP_CXX_FLAGS "-Wall -Wextra -std=c++14 -ggdb")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(lib/external/nitro/Nitro.cmake)

# add lib and bin directory
add_subdirectory(lib)
add_subdirectory(bin)
