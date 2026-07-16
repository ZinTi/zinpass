#!/usr/bin/env bash
# yaml-cpp Linux GCC 构建脚本

set -e

# 配置 Release 版本
cmake -B build/release/yaml-cpp \
      -G "Unix Makefiles" \
      -S yaml-cpp \
      -DCMAKE_INSTALL_PREFIX="$(pwd)/lib/yaml-cpp" \
      -DCMAKE_CXX_COMPILER=g++ \
      -DCMAKE_C_COMPILER=gcc \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF

# 并行构建并安装
cmake --build build/release/yaml-cpp --target install --parallel $(nproc) --verbose

echo "yaml-cpp 构建完成，安装至 $(pwd)/lib/yaml-cpp"
