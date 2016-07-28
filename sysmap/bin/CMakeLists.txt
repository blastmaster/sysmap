cmake_minimum_required(VERSION 3.2.2)

set(SYSMAP_SOURCES
    "sysmap.cc"
)

set(CMAKE_CXX_FLAGS ${SYSMAP_CXX_FLAGS})

include_directories(
    ../lib/include
    ${Boost_INCLUDE_DIRS}
)

add_executable(sysmap ${SYSMAP_SOURCES})

target_link_libraries(sysmap libsysmap
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

install(TARGETS sysmap DESTINATION bin)
