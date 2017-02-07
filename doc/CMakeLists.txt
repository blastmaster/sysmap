# creating a sphinx target to direct sphinx how to generate the documentation

message("ATTENTION Entering doc/")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# set sphinx source directory
set(SPHINX_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
# set sphinx build directory
set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build")
# set sphinx cache directory
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/build/doctrees")
# set sphinx html directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/build/html")

message("SPHINX_EXECUTABLE: ${SPHINX_EXECUTABLE}")
message("BINARY_BUILD_DIR: ${BINARY_BUILD_DIR}")
message("SPHINX_SOURCE_DIR: ${SPHINX_SOURCE_DIR}")
message("SPHINX_CACHE_DIR: ${SPHINX_CACHE_DIR}")
message("SPHINX_HTML_DIR: ${SPHINX_HTML_DIR}")

# allow cmake to control the configuration of sphinx
configure_file("${SPHINX_SOURCE_DIR}/conf.py.in"
               "${BINARY_BUILD_DIR}/conf.py" @ONLY)

#ATTENTION Entering doc/
#CMAKE_CURRENT_BINARY_DIR: /home/soeste/code/ada-fs/sysmap/build/doc
#CMAKE_CURRENT_SOURCE_DIR: /home/soeste/code/ada-fs/sysmap/doc
#SPHINX_EXECUTABLE: /home/soeste/code/ada-fs/sysmap/python/env/bin/sphinx-build
#BINARY_BUILD_DIR: /home/soeste/code/ada-fs/sysmap/build/doc/build
#SPHINX_SOURCE_DIR: /home/soeste/code/ada-fs/sysmap/doc/source
#SPHINX_CACHE_DIR: /home/soeste/code/ada-fs/sysmap/build/doc/build/doctrees
#SPHINX_HTML_DIR: /home/soeste/code/ada-fs/sysmap/build/doc/build/html

set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in")

set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

# TODO: directories must be separated by space, we get ";"s.
message("IN DOC DOXY_INPUT_FILES: ${DOXY_INPUT_FILES}")

configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY)

# add a cmake target for generating the doxygen API-Documentation for the C++ parts.
add_custom_target(doxygen_doc ALL
    ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMENT "Building Doxygen documentation")


# add a cmake target for generating the documentation
add_custom_target(sysmap_docs ALL
    ${SPHINX_EXECUTABLE} -b html -c "${BINARY_BUILD_DIR}" -d "${SPHINX_CACHE_DIR}" "${SPHINX_SOURCE_DIR}" "${SPHINX_HTML_DIR}"
    COMMENT "Building Sphinx documentation")

