# handle thrird-party dependencies.

message("cmake current list dir: ${CMAKE_CURRENT_LIST_DIR}")
message("cmake source dir: ${CMAKE_SOURCE_DIR}")
message("cmake current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

# UPDATING:
# Updating is done by changing the "GIT_TAG" of an ExternalProject to another Commit Hash.
# For finding a newer Commit no Browser is needed, the cloned Repositorys are often up to date,
# but checked out to the defined Commit Hash. That means they could allready contain newer Commits.
# If that is not the case a "git fetch" in the specific Repo will fetch newer commits.

# EXAMPLE:
# Updating yaml-cpp (starting from sysmap source dir):
# cd external/yaml-cpp/src/yaml-cpp-project/
# git log --all # Shows the newest commits, if HEAD is allready set to the newest perform a "git fetch".
#               # A simple "git log" would not show newer commits.
#
# Now pick a new Commit Hash and replace it inside this file with the old Hash after "GIT_TAG"
# Move into the sysmap/build directory and perform a "make". The ExternalProject and sysmap
# will be rebuild automaticly.
# If you are happy with the result just commit and push the changes made to this CMakeLists.txt file

include(ExternalProject)
#Clone and Build
ExternalProject_Add(yaml-cpp-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/yaml-cpp"
    GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp"
    GIT_PROGRESS 1
    GIT_TAG f5d5604a2cab5bd21152b32b67be0b1b179ecdce
    CMAKE_ARGS
        -DYAML_CPP_BUILD_TESTS=OFF
        -DYAML_CPP_BUILD_TOOLS=OFF
        -DYAML_CPP_BUILD_CONTRIB=OFF
    INSTALL_COMMAND ""
)

#Create Target
set(YAML_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/yaml-cpp/src/yaml-cpp-project/include)
set(YAML_LIB
    "${PROJECT_SOURCE_DIR}/external/yaml-cpp/src/yaml-cpp-project-build/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX}")
message("yaml-cpp: ${YAML_INCLUDE_DIR}")

message("check for yaml-cpp target")
if(NOT TARGET YAML::YAML)
    message("yaml-cpp target not found")
    add_library(YAML::YAML UNKNOWN IMPORTED)
    set_target_properties(YAML::YAML PROPERTIES IMPORTED_GLOBAL TRUE
        INTERFACE_INCLUDE_DIRECTORIES ${YAML_INCLUDE_DIR}
        IMPORTED_LOCATION ${YAML_LIB}
    )
endif()

# nitro
ExternalProject_Add(nitro-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/nitro"
    GIT_REPOSITORY "https://github.com/tud-zih-energy/nitro"
    GIT_PROGRESS 1
    GIT_TAG 0aab3e94f330ba3cd3e7f0ff61578bb3271a6fdd
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(NITRO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/nitro/src/nitro-project/include/)
message("nitro: ${NITRO_INCLUDE_DIR}")

message("check for nitro target")
if(NOT TARGET Nitro::Nitro)
    message("nitro target not found")
    add_library(Nitro::Nitro INTERFACE IMPORTED)
    set_target_properties(Nitro::Nitro PROPERTIES IMPORTED_GLOBAL TRUE
        INTERFACE_INCLUDE_DIRECTORIES ${NITRO_INCLUDE_DIR}
    )
endif()

# rapidjson
ExternalProject_Add(rapidjson-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/rapidjson"
    GIT_REPOSITORY "https://github.com/Tencent/rapidjson"
    GIT_PROGRESS 1
    GIT_TAG 0163a53f4a1c72e6a05848a63d80eee0d8e3f387
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(RAPIDJSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/rapidjson/src/rapidjson-project/include/)
message("rapidjson: ${RAPIDJSON_INCLUDE_DIR}")

message("check for rapidjson target")
if(NOT TARGET RapidJSON::RapidJSON)
    message("rapidjson target not found")
    add_library(RapidJSON::RapidJSON INTERFACE IMPORTED)
    set_target_properties(RapidJSON::RapidJSON PROPERTIES IMPORTED_GLOBAL TRUE
        INTERFACE_INCLUDE_DIRECTORIES ${RAPIDJSON_INCLUDE_DIR}
    )
endif()

# pugixml
ExternalProject_Add(pugixml-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/pugixml"
    GIT_REPOSITORY "https://github.com/zeux/pugixml"
    GIT_PROGRESS 1
    GIT_TAG 38edf255aee473ade74d955faa79e4f7a5c38eb1
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(PUGIXML_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/pugixml/src/pugixml-project/src/)
message("pugixml: ${PUGIXML_INCLUDE_DIR}")

message("check for pugixml target")
if(NOT TARGET PugiXML::PugiXML)
    message("pugixml target not found")
    add_library(PugiXML::PugiXML INTERFACE IMPORTED)
    set_target_properties(PugiXML::PugiXML PROPERTIES IMPORTED_GLOBAL TRUE
        INTERFACE_INCLUDE_DIRECTORIES ${PUGIXML_INCLUDE_DIR}
    )
endif()


# sqlite_modern_cpp
ExternalProject_Add(sqlite_modern_cpp-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/sqlite_modern_cpp"
    GIT_REPOSITORY "https://github.com/aminroosta/sqlite_modern_cpp"
    GIT_PROGRESS 1
    GIT_TAG e2248fae15c9e1a240f54d29a148e501f4ea2347
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(SQLITE_MODERN_CPP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sqlite_modern_cpp/hdr/)
message("sqlite_modern_cpp: ${SQLITE_MODERN_CPP_INCLUDE_DIR}")

message("check for sqlite_modern_cpp target")
if(NOT TARGET SQLITE_MODERN_CPP::SQLITE_MODERN_CPP)
    message("sqlite_modern_cpp target not found")
    add_library(SQLITE_MODERN_CPP::SQLITE_MODERN_CPP INTERFACE IMPORTED)
    set_target_properties(SQLITE_MODERN_CPP::SQLITE_MODERN_CPP PROPERTIES IMPORTED_GLOBAL TRUE
        INTERFACE_INCLUDE_DIRECTORIES ${SQLITE_MODERN_CPP_INCLUDE_DIR}
    )
endif()
