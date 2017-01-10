cmake_minimum_required(VERSION 3.2.2)

set(EXEC_TEST_STDIN_SOURCES
    "exec_stdin_test.cc"
)

set(EXEC_EACH_LINE_TEST_SOURCES
    "exec_each_line.cc"
)

set(DB_TEST_SOURCES
    "db_test.cc"
)

set(JSON_TEST_SOURCES
    "json_test.cc"
)

set(REGISTRY_TEST_SOURCES
    "register_test.cc"
)

set(CMAKE_CXX_FLAGS ${TEST_CXX_FLAGS})

include_directories(
    ../lib/include
    ${RAPIDJSON_INCLUDE_DIR}
    ${SQLITE_MODERN_CPP_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_executable(exec_stdin_test ${EXEC_TEST_STDIN_SOURCES})
add_executable(exec_each_line_test ${EXEC_EACH_LINE_TEST_SOURCES})
add_executable(db_test ${DB_TEST_SOURCES})
add_executable(json_test ${JSON_TEST_SOURCES})
add_executable(register_test ${REGISTRY_TEST_SOURCES})

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

target_link_libraries(db_test libsysmap
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(json_test libsysmap
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${SQLITE3_LIBRARY}
)

target_link_libraries(register_test libsysmap
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${SQLITE3_LIBRARY}
)
