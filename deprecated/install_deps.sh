#!/bin/env bash

# 该脚本用于下载并安装项目所需依赖（Windows 环境使用 git bash 执行）

# 检查必须 3 个参数（$# 不含 $0）
if [ $# -ne 3 ]; then
    echo "Usage: $0 <os_name> <c_compiler> <workspace_dir>"
    echo "os_name: ubuntu-latest or windows-latest"
    echo "c_compiler: gcc, clang, or cl"
    exit 1
fi
os_name=$1
c_compiler=$2
workspace_dir=$3

mkdir -p "${workspace_dir}/third_party/"

# 下载第三方库预编译文件
if [ "${os_name}" == "ubuntu-latest" ] && [ "${c_compiler}" == "gcc" ]; then
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/sqlite/sqlite-v3.50.4-20251021-linux-x86_64-gcc.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/openssl/openssl-v3.5.4-20251021-linux-x86_64-gcc.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/grpc/grpc-v1.76.0-20251125-linux-x86_64-gcc.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/yaml-cpp/yaml-cpp-v0.9.0-20260226-linux-x86_64-gcc.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/boost/boost-v1.90.0-20260226-linux-x86_64-gcc.tar.xz

  tar -xf sqlite-v3.50.4-20251021-linux-x86_64-gcc.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf openssl-v3.5.4-20251021-linux-x86_64-gcc.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf grpc-v1.76.0-20251125-linux-x86_64-gcc.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf yaml-cpp-v0.9.0-20260226-linux-x86_64-gcc.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf boost-v1.90.0-20260226-linux-x86_64-gcc.tar.xz -C "${workspace_dir}/third_party/"

elif [ "${os_name}" == "ubuntu-latest" ] && [ "${c_compiler}" == "clang" ]; then
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/sqlite/sqlite-v3.50.4-20251021-linux-x86_64-clang.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/openssl/openssl-v3.5.4-20251021-linux-x86_64-clang.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/grpc/grpc-v1.76.0-20251125-linux-x86_64-clang.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/yaml-cpp/yaml-cpp-v0.9.0-20260226-linux-x86_64-clang.tar.xz
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/boost/boost-v1.90.0-20260226-linux-x86_64-clang.tar.xz

  tar -xf sqlite-v3.50.4-20251021-linux-x86_64-clang.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf openssl-v3.5.4-20251021-linux-x86_64-clang.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf grpc-v1.76.0-20251125-linux-x86_64-clang.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf yaml-cpp-v0.9.0-20260226-linux-x86_64-clang.tar.xz -C "${workspace_dir}/third_party/"
  tar -xf boost-v1.90.0-20260226-linux-x86_64-clang.tar.xz -C "${workspace_dir}/third_party/"

elif [ "${os_name}" == "windows-latest" ] && [ "${c_compiler}" == "cl" ]; then
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/sqlite/sqlite-v3.50.4-20251021-windows-x86_64-msvc.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/openssl/openssl-v3.5.4-20251021-windows-x86_64-msvc.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/grpc/grpc-v1.76.0-20251125-windows-x86_64-msvc.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/yaml-cpp/yaml-cpp-v0.9.0-20260226-windows-x86_64-msvc.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/boost/boost-v1.90.0-20260226-windows-x86_64-msvc.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/qt/qt-release-6.10.0-20260217-windows-x86_64-msvc.7z

  7z x sqlite-v3.50.4-20251021-windows-x86_64-msvc.7z -o"${workspace_dir}/third_party/"
  7z x openssl-v3.5.4-20251021-windows-x86_64-msvc.7z -o"${workspace_dir}/third_party/"
  7z x grpc-v1.76.0-20251125-windows-x86_64-msvc.7z -o"${workspace_dir}/third_party/"
  7z x yaml-cpp-v0.9.0-20260226-windows-x86_64-msvc.7z -o"${workspace_dir}/third_party/"
  7z x boost-v1.90.0-20260226-windows-x86_64-msvc.7z -o"${workspace_dir}/third_party/"
  7z x qt-release-6.10.0-20260217-windows-x86_64-msvc.7z -o"${workspace_dir}/third_party/"

elif [ "${os_name}" == "windows-latest" ] && [ "${c_compiler}" == "gcc" ]; then
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/sqlite/sqlite-v3.50.4-20251021-windows-x86_64-mingw.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/openssl/openssl-v3.5.4-20251021-windows-x86_64-msys2.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/grpc/grpc-v1.76.0-20251125-windows-x86_64-mingw.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/yaml-cpp/yaml-cpp-v0.9.0-20260226-windows-x86_64-mingw.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/boost/boost-v1.90.0-20260226-windows-x86_64-msys2.7z
  curl -L --retry 3 -O https://github.com/ZinTi/cpp-prebuilt-libs/releases/download/qt/qt-release-6.10.0-20260217-windows-x86_64-mingw.7z

  7z x sqlite-v3.50.4-20251021-windows-x86_64-mingw.7z -o"${workspace_dir}/third_party/"
  7z x openssl-v3.5.4-20251021-windows-x86_64-msys2.7z -o"${workspace_dir}/third_party/"
  7z x grpc-v1.76.0-20251125-windows-x86_64-mingw.7z -o"${workspace_dir}/third_party/"
  7z x yaml-cpp-v0.9.0-20260226-windows-x86_64-mingw.7z -o"${workspace_dir}/third_party/"
  7z x boost-v1.90.0-20260226-windows-x86_64-msys2.7z -o"${workspace_dir}/third_party/"
  7z x qt-release-6.10.0-20260217-windows-x86_64-mingw.7z -o"${workspace_dir}/third_party/"

fi

# 重命名文件夹
mv "${workspace_dir}"/third_party/sqlite-* "${workspace_dir}"/third_party/sqlite
mv "${workspace_dir}"/third_party/openssl-* "${workspace_dir}"/third_party/openssl
mv "${workspace_dir}"/third_party/grpc-* "${workspace_dir}"/third_party/grpc
mv "${workspace_dir}"/third_party/yaml-cpp-* "${workspace_dir}"/third_party/yaml-cpp
mv "${workspace_dir}"/third_party/boost-* "${workspace_dir}"/third_party/boost
if [ "${os_name}" == "windows-latest" ]; then
  mv "${workspace_dir}"/third_party/qt-* "${workspace_dir}"/third_party/qt
fi

# 清理下载的压缩包，安装其他依赖
if [ "${os_name}" == "ubuntu-latest" ]; then
  rm *.tar.xz

  sudo apt update
  # Linux 上 Qt6 依赖 GLIB2、OpenGL、PNG 等库
  sudo apt install -y \
      bison \
      flex \
      gperf \
      libalsa-ocaml-dev \
      libclang-dev \
      libdbus-1-dev \
      libfontconfig1-dev \
      libgl1-mesa-dev \
      libglib2.0-dev \
      libglu1-mesa-dev \
      libgstreamer1.0-dev \
      libgstreamer-plugins-base1.0-dev \
      libharfbuzz-dev \
      libjpeg-dev \
      libmpc-dev \
      libnss3-dev \
      libpng-dev \
      libpulse-dev \
      libssl-dev \
      libsystemd-dev \
      libtiff-dev \
      libudev-dev \
      libx11-dev \
      libxcb1-dev \
      libxcb-glx0-dev \
      libxcursor-dev \
      libxext-dev \
      libxinerama-dev \
      libxkbcommon-dev \
      libxrandr-dev \
      llvm \
      mesa-common-dev \
      ninja-build

else
  rm *.7z
fi

ls -al "${workspace_dir}/third_party/"
