# Build Zinpass for Windows using MSVC (Visual Studio)

# Configure CMake project
# 对于 Visual Studio 额外指定架构，且不在生成阶段指定 -DCMAKE_BUILD_TYPE ，而是在 Build 阶段指定 --config 参数
# cmake -B <build-dir> -G <generator-name> -S <source-dir> -DCMAKE_INSTALL_PREFIX=<install-dir> -A <arch for vs>
cmake -B "build" `
    -G "Visual Studio 17 2022" `
    -S "." `
    -DCMAKE_INSTALL_PREFIX="install" `
    -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl `
    -DCMAKE_C_STANDARD=17 -DCMAKE_CXX_STANDARD=17 `
    -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain.cmake" `
    -A "x64" `
    -DBUILD_CLIENT_DESKTOP=OFF

# Build and install
cmake --build "build" --target install --config Release --parallel 12
