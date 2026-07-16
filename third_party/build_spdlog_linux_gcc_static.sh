#!/usr/bin/env bash
# spdlog Linux GCC 构建脚本 (静态库)

set -e

# 配置 Release 版本
cmake -B build/release/spdlog \
      -G "Unix Makefiles" \
      -S spdlog \
      -DCMAKE_INSTALL_PREFIX="$(pwd)/lib/spdlog" \
      -DCMAKE_CXX_COMPILER=g++ \
      -DCMAKE_C_COMPILER=gcc \
      -DCMAKE_BUILD_TYPE=Release \
      -DSPDLOG_BUILD_SHARED=OFF \
      -DSPDLOG_BUILD_TESTS=OFF \
      -DSPDLOG_BUILD_EXAMPLE=OFF \
      -DSPDLOG_INSTALL=ON

# 并行构建并安装
cmake --build build/release/spdlog --target install --parallel $(nproc) --verbose

echo "spdlog 构建完成，安装至 $(pwd)/lib/spdlog"
