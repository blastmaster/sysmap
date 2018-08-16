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
set(YAML_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/yaml-cpp/src/yaml-cpp-project/include CACHE PATH "yaml-cpp include dir" FORCE)
add_library(yaml-cpp UNKNOWN IMPORTED)
set_target_properties(yaml-cpp PROPERTIES
    IMPORTED_LOCATION
    ${PROJECT_SOURCE_DIR}/external/yaml-cpp-project/src/yaml-cpp-project-build/${CMAKE_STATIC_LIBRARY_PREFIX}libyaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX}
    )

# nitro
ExternalProject_Add(nitro-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/nitro"
    GIT_REPOSITORY "https://github.com/tud-zih-energy/nitro"
    GIT_PROGRESS 1
    GIT_TAG 0aab3e94f330ba3cd3e7f0ff61578bb3271a6fdd
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(NITRO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/nitro/src/nitro-project/include/ CACHE PATH "nitro include dir" FORCE)
message("nitro: ${NITRO_INCLUDE_DIR}")

# rapidjson
ExternalProject_Add(rapidjson-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/rapidjson"
    GIT_REPOSITORY "https://github.com/Tencent/rapidjson"
    GIT_PROGRESS 1
    GIT_TAG 0163a53f4a1c72e6a05848a63d80eee0d8e3f387
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(RAPIDJSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/rapidjson/src/rapidjson-project/include/ CACHE PATH "rapidjson include dir" FORCE)
message("rapidjson: ${RAPIDJSON_INCLUDE_DIR}")

# pugixml
ExternalProject_Add(pugixml-project
    PREFIX "${PROJECT_SOURCE_DIR}/external/pugixml"
    GIT_REPOSITORY "https://github.com/zeux/pugixml"
    GIT_PROGRESS 1
    GIT_TAG 38edf255aee473ade74d955faa79e4f7a5c38eb1
    INSTALL_COMMAND ""
)
set(PUGIXML_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/pugixml/src/pugixml-project/src/ CACHE PATH "pugixml include dir" FORCE)
message("pugixml: ${PUGIXML_INCLUDE_DIR}")

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
set(SQLITE_MODERN_CPP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sqlite_modern_cpp/hdr/ CACHE PATH "sqlite_modern_cpp include dir" FORCE)
message("sqlite_modern_cpp: ${SQLITE_MODERN_CPP_INCLUDE_DIR}")
