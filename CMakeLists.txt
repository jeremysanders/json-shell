cmake_minimum_required (VERSION 2.6)
project (json-shell)

# The version number.
set (JSONSHELL_VERSION_MAJOR 0)
set (JSONSHELL_VERSION_MINOR 1)

add_executable(reformatter reformatter.cpp yajl.cpp)

target_link_libraries (reformatter yajl)

