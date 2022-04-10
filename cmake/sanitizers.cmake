# Sanitizers

# ------------------------------------------------------------------------------

# Address Sanitizer
# https://clang.llvm.org/docs/AddressSanitizer.html

set(ASAN_COMPILE_FLAGS -fsanitize=address -fno-sanitize-recover=all)
set(ASAN_LINK_FLAGS -fsanitize=address)

if(ASAN)
  message(STATUS "Sanitize with Address Sanitizer (ASAN)")
  add_compile_options(${ASAN_COMPILE_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
endif()

# ------------------------------------------------------------------------------

# Leak Sanitizer
# https://clang.llvm.org/docs/LeakSanitizer.html

set(LSAN_COMPILE_FLAGS -fsanitize=leak -fno-sanitize-recover=all)
set(LSAN_LINK_FLAGS -fsanitize=leak)

if(LSAN)
  message(STATUS "Sanitize with Leak Sanitizer (LSAN)")
  add_compile_options(${LSAN_COMPILE_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LSAN_LINK_FLAGS}")
endif()

# ------------------------------------------------------------------------------

# Memory Sanitizer
# https://clang.llvm.org/docs/MemorySanitizer.html

set(MSAN_COMPILE_FLAGS -fsanitize=memory -fno-sanitize-recover=all)
set(MSAN_LINK_FLAGS -fsanitize=memory)

if(MSAN)
  message(STATUS "Sanitize with Memory Sanitizer (MSAN)")
  add_compile_options(${MSAN_COMPILE_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSAN_LINK_FLAGS}")
endif()

# ------------------------------------------------------------------------------

# Thread Sanitizer
# https://clang.llvm.org/docs/ThreadSanitizer.html

set(TSAN_COMPILE_FLAGS -fsanitize=thread -fno-sanitize-recover=all)
set(TSAN_LINK_FLAGS -fsanitize=thread)

if(TSAN)
  message(STATUS "Sanitize with Thread Sanitizer (TSAN)")
  add_compile_options(${TSAN_COMPILE_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TSAN_LINK_FLAGS}")
endif()

# ------------------------------------------------------------------------------

# UB Sanitizer
# https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html

set(UBSAN_COMPILE_FLAGS -fsanitize=undefined -fno-sanitize-recover=all)
set(UBSAN_LINK_FLAGS -fsanitize=undefined)

if(UBSAN)
  message(STATUS "Sanitize with UB Sanitizer (UBSAN)")
  add_compile_options(${UBSAN_COMPILE_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${UBSAN_LINK_FLAGS}")
endif()
