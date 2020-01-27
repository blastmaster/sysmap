cmake_minimum_required(VERSION 3.10.1)

include(ExternalProject)
ExternalProject_Add(catch-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/Catch"
    GIT_REPOSITORY "https://github.com/catchorg/Catch2"
    GIT_PROGRESS 1
    GIT_TAG c6178601c5c87b50e3114b8a8043d1ee6dfb4041
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/Catch/src/catch-project/include/ CACHE PATH "pugixml include dir" FORCE)
message("Catch: ${CATCH_INCLUDE_DIR}")

set(EXEC_TEST_STDIN_SOURCES
    "exec_stdin_test.cc"
)

set(EXEC_EACH_LINE_TEST_SOURCES
    "exec_each_line.cc"
)

# set(DB_TEST_SOURCES
#     "db_test.cc"
# )

set(JSON_TEST_SOURCES
    "json_test.cc"
)

set(REGISTRY_TEST_SOURCES
    "register_test.cc"
)

set(XML_TEST_SOURCES
    "xml_test.cc"
)

set(SUBDIR_TEST_SOURCES
    "subdirectory_test.cpp"
)

set(SYSQUERY_TEST_SOURCES
    "sysquery_test.cpp"
    )

set(FILESYSTEM_TEST_SOURCES
    "filesystem_test.cc"
    )

set(CMAKE_CXX_FLAGS ${TEST_CXX_FLAGS})

include_directories(
    ../lib/include
    ${Boost_INCLUDE_DIRS}
    ${CATCH_INCLUDE_DIR}
    $<TARGET_PROPERTY:SQLITE_MODERN_CPP::SQLITE_MODERN_CPP,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:PugiXML::PugiXML,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Nitro::Nitro,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:RapidJSON::RapidJSON,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:YAML::YAML,INTERFACE_INCLUDE_DIRECTORIES>
)

if(LSCPU_CMD)
    add_executable(test_lscpu_extractor "test_lscpu_extractor.cc")
    add_test(NAME test_lscpu_extractor COMMAND test_lscpu_extractor)
    add_dependencies(test_lscpu_extractor catch-project)
    target_link_libraries(test_lscpu_extractor libsysmap
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${SQLITE3_LIBRARY})
endif()

add_executable(exec_stdin_test ${EXEC_TEST_STDIN_SOURCES})
add_executable(exec_each_line_test ${EXEC_EACH_LINE_TEST_SOURCES})
# add_executable(db_test ${DB_TEST_SOURCES})
add_executable(json_test ${JSON_TEST_SOURCES})
add_executable(register_test ${REGISTRY_TEST_SOURCES})
add_executable(xml_test ${XML_TEST_SOURCES})
add_executable(sysquery_test ${SYSQUERY_TEST_SOURCES})
add_executable(filesystem_test ${FILESYSTEM_TEST_SOURCES})
add_dependencies(exec_stdin_test catch-project)
add_dependencies(exec_each_line_test catch-project)
add_dependencies(json_test catch-project)
add_dependencies(register_test catch-project)
add_dependencies(xml_test catch-project)
add_dependencies(sysquery_test catch-project)
add_dependencies(filesystem_test catch-project)

add_test(NAME exec_stdin_test COMMAND exec_stdin_test)
add_test(NAME exec_each_line_test COMMAND exec_each_line_test)
# add_test(NAME db_test COMMAND db_test)
add_test(NAME json_test COMMAND json_test)
add_test(NAME register_test COMMAND register_test)
add_test(NAME xml_test COMMAND xml_test)
add_test(NAME sysquery_test COMMAND sysquery_test)
add_test(NAME filesystem_test COMMAND filesystem_test)

target_link_libraries(exec_stdin_test libsysmap
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(exec_each_line_test libsysmap
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${SQLITE3_LIBRARY}
)

# target_link_libraries(db_test libsysmap
    # ${Boost_PROGRAM_OPTIONS_LIBRARY}
    # ${Boost_SYSTEM_LIBRARY}
    # ${Boost_FILESYSTEM_LIBRARY}
    # ${SQLITE3_LIBRARY}
# )

target_link_libraries(json_test libsysmap
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(register_test libsysmap
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(xml_test libsysmap
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(sysquery_test
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(filesystem_test
    libsysmap
)
