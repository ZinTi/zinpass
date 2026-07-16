#!/usr/bin/env bash
# gRPC Linux GCC 构建脚本

set -e

# 应用 re2 头文件补丁（修复缺失 cstdint）
# if [ -f grpc/third_party/re2/util/pcre.h ]; then
#     sed -i '1i #include <cstdint>' grpc/third_party/re2/util/pcre.h
#     echo "已应用 re2 补丁"
# fi

# 配置 Release 版本
cmake -B build/release/grpc \
      -G "Unix Makefiles" \
      -S grpc \
      -DCMAKE_INSTALL_PREFIX="$(pwd)/lib/grpc" \
      -DCMAKE_CXX_COMPILER=g++ \
      -DCMAKE_C_COMPILER=gcc \
      -DCMAKE_C_STANDARD=17 \
      -DCMAKE_CXX_STANDARD=17 \
      -DOPENSSL_NO_ASM=ON \
      -DOPENSSL_NO_ADX=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DBUILD_SHARED_LIBS=OFF \
      -DgRPC_INSTALL=ON \
      -DgRPC_ABSL_PROVIDER=module \
      -DgRPC_CARES_PROVIDER=module \
      -DgRPC_PROTOBUF_PROVIDER=module \
      -DgRPC_RE2_PROVIDER=module \
      -DgRPC_SSL_PROVIDER=module \
      -DgRPC_ZLIB_PROVIDER=module \
      -DCMAKE_BUILD_TYPE=Release

# 并行构建并安装
cmake --build build/release/grpc --target install --parallel $(nproc) --verbose

echo "gRPC 构建完成，安装至 $(pwd)/lib/grpc"
