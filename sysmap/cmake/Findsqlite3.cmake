# try to find sqlite library

set(_sqlite3_HEADER_SEARCH_DIRS
        "/usr/include"
        "/usr/local/include")

set(_sqlite3_LIBRARY_SEARCH_DIRS
        "/usr/lib"
        "/usr/lib64"
        "/usr/lib/x86_64-linux-gnu")

find_path(SQLITE3_INCLUDE_DIR "sqlite3.h" PATHS ${_sqlite3_HEADER_SEARCH_DIRS})
find_library(SQLITE3_LIBRARY names "libsqlite3" "sqlite3" HINTS ${_sqlite3_LIBRARY_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLITE3 DEFAULT_MSG SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

mark_as_advanced(SQLITE3_INCLUDE_DIR SQLITE3_LIBRARY)
set(SQLITE3_LIBRARIES "${SQLITE_LIBRARY}")
set(SQLITE3_INCLUDE_DIRS "${SQLITE_INCLUDE_DIR}")
