cmake_minimum_required(VERSION 3.10.1)

add_executable(sysmap
    "sysmap.cc"
)

target_include_directories(sysmap
    PRIVATE
        ../lib/include
        ${Boost_INCLUDE_DIRS}
        ${SQLITE3_INCLUDE_DIR}
        ${HWLOC_INCLUDE_DIRS}
        $<TARGET_PROPERTY:RapidJSON::RapidJSON,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:SQLITE_MODERN_CPP::SQLITE_MODERN_CPP,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:YAML::YAML,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Nitro::Nitro,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:PugiXML::PugiXML,INTERFACE_INCLUDE_DIRECTORIES>
)

if(BUILD_STATIC)
    target_link_libraries(sysmap
        PRIVATE
            -Wl,--whole-archive libsysmap -Wl,--no-whole-archive
            ${Boost_LIBRARIES}
            ${SQLITE3_LIBRARY}
            ${PUGIXML_LIBRARY}
    )

else()
    target_link_libraries(sysmap
        PRIVATE
            libsysmap
            ${Boost_LIBRARIES}
            ${SQLITE3_LIBRARY}
            ${PUGIXML_LIBRARY}
    )
endif()

target_compile_options(sysmap
    PUBLIC
        -Wextra -std=c++14 -D PUGIXML_HEADER_ONLY
)

install(TARGETS sysmap DESTINATION bin)
