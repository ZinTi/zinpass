#!/bin/env bash
# Build Zinpass for Linux using GCC

set -e

# Configure CMake project
# cmake -B <build-dir> -G <generator-name> -S <source-dir> -DCMAKE_INSTALL_PREFIX=<install-dir> -DCMAKE_BUILD_TYPE=<type>
cmake -B "build" \
    -G "Unix Makefiles" \
    -S . \
    -DCMAKE_INSTALL_PREFIX="install" \
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_C_STANDARD=17 -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG" \
    -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain.cmake" \
    -DCMAKE_INSTALL_RPATH='$ORIGIN:$ORIGIN/../lib' \
    -DBUILD_SERVER=ON \
    -DBUILD_CONTROL=ON \
    -DBUILD_CLIENT_DESKTOP=ON

# Build And Install
cmake --build "build" --target install --parallel 12
