# Clang-tools utils

# ------------------------------------------------------------------------------

# clang-format
# https://clang.llvm.org/docs/ClangFormat.html

set(CLANG_FORMAT_FLAGS -i -style=file)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
  )

  add_custom_target(
    format
    COMMAND clang-format
    ${ALL_CXX_SOURCE_FILES}
    ${CLANG_FORMAT_FLAGS}
  )
endif()

# ------------------------------------------------------------------------------

# clang-tidy
# https://clang.llvm.org/extra/clang-tidy/

set(CLANG_TIDY_FLAGS -format-style=file)
set(CLANG_TIDY_EXTRA_FLAGS --std=c++20)

find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
  )

  add_custom_target(
    tidy
    COMMAND clang-tidy
    ${ALL_CXX_SOURCE_FILES}
    ${CLANG_TIDY_FLAGS}
    --
    ${CLANG_TIDY_EXTRA_FLAGS}
  )
endif()

# ------------------------------------------------------------------------------

# clang-check
# https://clang.llvm.org/docs/ClangCheck.html

set(CLANG_CHECK_FLAGS -analyze -ast-dump)

find_program(CLANG_CHECK "clang-check")
if(CLANG_CHECK)
  file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
  )

  add_custom_target(
    check
    COMMAND clang-check
    ${ALL_CXX_SOURCE_FILES}
    ${CLANG_CHECK_FLAGS}
    --
  )
endif()
