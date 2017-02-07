#FindHwloc - tries to find the hwloc library.

# TODO if pkgconfig is available use pkg-config

# Default search paths
set(_hwloc_HEADER_SEARCH_DIRS
        "/usr/include"
        "/usr/include/hwloc"
        "/usr/local/include"
        "/usr/local/include/hwloc")

set(_hwloc_LIBRARY_SEARCH_DIRS
        "/usr/lib"
        "/usr/lib64"
        "/usr/lib/x86_64-linux-gnu")


find_path(HWLOC_INCLUDE_DIR "hwloc.h" PATHS ${_hwloc_HEADER_SEARCH_DIRS})
find_library(HWLOC_LIBRARY NAMES "libhwloc" "hwloc" HINTS ${_hwloc_LIBRARY_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HWLOC DEFAULT_MSG HWLOC_LIBRARY HWLOC_INCLUDE_DIR)

mark_as_advanced(HWLOC_INCLUDE_DIR HWLOC_LIBRARY)
set(HWLOC_LIBRARIES "${HWLOC_LIBRARY}")
set(HWLOC_INCLUDE_DIRS "${HWLOC_INCLUDE_DIR}")
