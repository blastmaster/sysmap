cmake_minimum_required(VERSION 3.2.2)

set(SYSMAP_SOURCES
    "sysmap.cc"
)

set(CMAKE_CXX_FLAGS ${SYSMAP_CXX_FLAGS})

include_directories(
    ../lib/include
    ${Boost_INCLUDE_DIRS}
    ${RAPIDJSON_INCLUDE_DIR}
    ${SQLITE_MODERN_CPP_INCLUDE_DIR}
    ${PUGIXML_INCLUDE_DIR}
    ${YAML_CPP_SOURCE_DIR}/include
)

add_executable(sysmap ${SYSMAP_SOURCES})

if(BUILD_STATIC)
    target_link_libraries(sysmap
        -Wl,--whole-archive libsysmap -Wl,--no-whole-archive
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${SQLITE3_LIBRARY}
        ${PUGIXML_LIBRARY}
        yaml-cpp
    )
else()
    target_link_libraries(sysmap
        libsysmap
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${SQLITE3_LIBRARY}
        ${PUGIXML_LIBRARY}
        yaml-cpp
    )
endif()


install(TARGETS sysmap DESTINATION bin)
