# handle thrird-party dependencies.

message("cmake current list dir: ${CMAKE_CURRENT_LIST_DIR}")
message("cmake source dir: ${CMAKE_SOURCE_DIR}")
message("cmake current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

# nitro
message(STATUS "looking for nitro")
file(GLOB NITRO_FILES "nitro/*")
list(LENGTH NITRO_FILES COUNT_NITRO_FILES)
if (${COUNT_NITRO_FILES} EQUAL 0)
    message(STATUS "Unable to find nitro, cloning...")
    execute_process(COMMAND git submodule update --init -- nitro
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
#include(nitro/Nitro.cmake HAVE_NITRO)

# rapidjson
message(STATUS "looking for rapidjson")
#find_path(RAPIDJSON_INCLUDE_DIR )
if ((NOT RAPIDJSON_INCLUDE_DIR) OR (NOT EXISTS ${RAPIDJSON_INCLUDE_DIR}))
    message("Unable to find rapidjson, cloning...")
    execute_process(COMMAND git submodule update --init -- rapidjson
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include/
        CACHE PATH "rapidjson include directory")
endif()
# TODO: destination needed
#install(DIRECTORY ${RAPIDJSON_INCLUDE_DIR}/rapidjson DESTINATION include)



# sqlite_modern_cpp
message(STATUS "looking for sqlite_modern_cpp")
if ((NOT SQLITE_MODERN_CPP_INCLUDE_DIR) OR (NOT EXISTS ${SQLITE_MODERN_CPP_INCLUDE_DIR}))
    message(STATUS "Unable to find sqlite_modern_cpp, cloning...")
    execute_process(COMMAND git submodule update --init -- sqlite_modern_cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(SQLITE_MODERN_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sqlite_modern_cpp/hdr/
        CACHE PATH "sqlite_modern_cpp include directory")
endif()
# TODO: destination needed
#install(DIRECTORY ${SQLITE_MODERN_CPP_INCLUDE_DIR} DESTINATION include)

# pugixml
message(STATUS "looking for pugixml")
if ((NOT PUGIXML_INCLUDE_DIR) OR (NOT EXISTS ${PUGIXML_INCLUDE_DIR}))
    message(STATUS "Unable to find pugixml, cloning...")
    execute_process(COMMAND git submodule update --init -- pugixml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(PUGIXML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pugixml/src/
        CACHE PATH "pugixml include directory")
endif()
# TODO: destination needed
#install(DIRECTORY ${PUGIXML_INCLUDE_DIR} DESTINATION include)

message("rapidjson: ${RAPIDJSON_INCLUDE_DIR}")
message("sqlite_modern_cpp: ${SQLITE_MODERN_CPP_INCLUDE_DIR}")
message("pugixml: ${PUGIXML_INCLUDE_DIR}")

# catch
message("looking for catch...")

file(GLOB CATCH_FILES "Catch/*")
list(LENGTH CATCH_FILES COUNT_CATCH_FILES)
if (${COUNT_CATCH_FILES} EQUAL 0)
    message(STATUS "Unable to find catch, cloning...")
    execute_process(COMMAND git submodule update --init -- Catch
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include/
    CACHE PATH "catch include directory")

message("CATCH INCLUDE DIR: ${CATCH_INCLUDE_DIR}")
