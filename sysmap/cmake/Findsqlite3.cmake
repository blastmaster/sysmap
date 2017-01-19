# try to find sqlite library

set(_sqlite3_HEADER_SEARCH_DIRS
        "/usr/include"
        "/usr/local/include")

set(_sqlite3_LIBRARY_SEARCH_DIRS
        "/usr/lib"
        "/usr/lib64"
        "/usr/lib/x86_64-linux-gnu")

# environment variable
set(_sqlite3_ENV_ROOT "$ENV{SQLITE3_ROOT}")

# if no root is given by user, use environment
if(NOT SQLITE3_ROOT_DIR AND _sqlite3_ENV_ROOT)
    set(SQLITE3_ROOT_DIR "${_sqlite3_ENV_ROOT}")
endif()

if(SQLITE3_ROOT_DIR)
    set(_sqlite3_HEADER_SEARCH_DIRS
        "${SQLITE3_ROOT_DIR}"
        "${SQLITE3_ROOT_DIR}/include"
        "{_sqlite3_HEADER_SEARCH_DIRS}")

    set(_sqlite3_LIBRARY_SEARCH_DIRS
        "${SQLITE3_ROOT_DIR}"
        "${SQLITE3_ROOT_DIR}/lib"
        "{_sqlite3_LIBRARY_SEARCH_DIRS}")
endif()

find_path(SQLITE3_INCLUDE_DIR NAMES "sqlite3.h" PATHS ${_sqlite3_HEADER_SEARCH_DIRS})
find_library(SQLITE3_LIBRARY NAMES "libsqlite3" "sqlite3" HINTS ${_sqlite3_LIBRARY_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLITE3 DEFAULT_MSG SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

mark_as_advanced(SQLITE3_INCLUDE_DIR SQLITE3_LIBRARY)
set(SQLITE3_LIBRARIES "${SQLITE3_LIBRARY}")
set(SQLITE3_INCLUDE_DIRS "${SQLITE3_INCLUDE_DIR}")
