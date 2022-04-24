add_executable(
  lib_test
  typelist_ut.cpp
)

set_target_properties(lib_test PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(lib_test lib gtest_main)

gtest_discover_tests(lib_test)
