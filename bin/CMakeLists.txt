cmake_minimum_required(VERSION 3.2.2)

add_executable(sysmap
    "sysmap.cc"
)

target_include_directories(sysmap
    PRIVATE
        ../lib/include
        ${NITRO_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${RAPIDJSON_INCLUDE_DIR}
        ${SQLITE_MODERN_CPP_INCLUDE_DIR}
        ${PUGIXML_INCLUDE_DIR}
        ${YAML_INCLUDE_DIR}
)

if(BUILD_STATIC)
    target_link_libraries(sysmap
        PRIVATE
            -Wl,--whole-archive libsysmap -Wl,--no-whole-archive
            ${Boost_PROGRAM_OPTIONS_LIBRARY}
            ${Boost_SYSTEM_LIBRARY}
            ${Boost_FILESYSTEM_LIBRARY}
            ${SQLITE3_LIBRARY}
            ${PUGIXML_LIBRARY}
    )

else()
    target_link_libraries(sysmap
        PRIVATE
            libsysmap
            ${Boost_PROGRAM_OPTIONS_LIBRARY}
            ${Boost_SYSTEM_LIBRARY}
            ${Boost_FILESYSTEM_LIBRARY}
            ${SQLITE3_LIBRARY}
            ${PUGIXML_LIBRARY}
    )
endif()

target_compile_options(sysmap
    PUBLIC
        -Wextra -std=c++14 -D PUGIXML_HEADER_ONLY
)

install(TARGETS sysmap DESTINATION bin)
