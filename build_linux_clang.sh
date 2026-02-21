#!/bin/env bash

# Build Zinpass for Linux using Clang

# 1. Generate gRPC build files
if command -v python &> /dev/null; then
    python scripts/protoc.py "third_party/grpc" -c 111
elif command -v python3 &> /dev/null; then
    python3 scripts/protoc.py "third_party/grpc" -c 111
else
    echo "Error: Neither python nor python3 found. Please install Python." >&2
    exit 1
fi

# 2. Configure CMake project
# cmake -B <build-dir> -G <generator-name> -S <source-dir> -DCMAKE_INSTALL_PREFIX=<install-dir> -DCMAKE_BUILD_TYPE=<type>
cmake -B "build" \
    -G "Unix Makefiles" \
    -S . \
    -DCMAKE_INSTALL_PREFIX="install" \
    -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_STANDARD=17 -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SERVICE=ON \
    -DBUILD_CONTROL=ON \
    -DBUILD_CLIENT_DESKTOP=OFF

# 3. Build And Install
cmake --build "build" --target install --parallel 2
