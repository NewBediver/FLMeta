#!/bin/bash

cmake \
  -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux_x64_clang.cmake \
  -B ../build \
  -S ..

cmake --build ../build
