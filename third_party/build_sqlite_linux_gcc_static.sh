#!/usr/bin/env bash
# SQLite Linux GCC 构建脚本 (使用 CMake)

set -e

# 配置 CMake
cmake -B build/release/sqlite \
    -G "Unix Makefiles" \
    -S sqlite \
    -DCMAKE_INSTALL_PREFIX="$(pwd)/lib/sqlite" \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF   # CMakeLists 已定义静态库，此项可省

# 并行编译并安装
cmake --build build/release/sqlite --target install --parallel $(nproc) --verbose

echo "SQLite 构建完成，安装至 $(pwd)/lib/sqlite"
