add_library(lib STATIC
  helpers.hpp
  typelist.hpp
)

set_target_properties(lib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(lib PRIVATE .)
