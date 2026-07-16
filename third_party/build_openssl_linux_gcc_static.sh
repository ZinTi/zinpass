#!/usr/bin/env bash
# OpenSSL Linux GCC 构建脚本

TOPDIR="$(pwd)"                     # 当前为 third_party 目录

# 创建并进入 build 目录
mkdir -p build/release/openssl
cd build/release/openssl

# 使用绝对路径调用 Configure，并指定目标平台
"$TOPDIR/openssl/Configure" \
    --prefix="$TOPDIR/lib/openssl" \
    --openssldir="$TOPDIR/lib/openssl/ssl" \
    -Wl,-rpath,'$(LIBRPATH)' \
    no-ssl3 \
    no-weak-ssl-ciphers \
    -DOPENSSL_USE_NODELETE \
    CC=gcc \
    CXX=g++

make -j$(nproc)
make install

echo "OpenSSL 构建完成，安装至 $TOPDIR/lib/openssl"
