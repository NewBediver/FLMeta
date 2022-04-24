cmake_minimum_required(VERSION 3.14)

project(fl_meta)

include(cmake/check_compiler.cmake)
include(cmake/clang-tools.cmake)
include(cmake/compile_options.cmake)
include(cmake/development.cmake)
include(cmake/project_preferences.cmake)
include(cmake/sanitizers.cmake)

add_subdirectory(external)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
