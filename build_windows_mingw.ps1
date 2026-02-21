# Build Zinpass for Windows using MinGW-w64

# 1. Generate gRPC build files
# windows git bash 环境下执行 protoc 命令会出现运行时库冲突，故使用 pwsh 执行
python scripts/protoc.py "third_party/grpc" -c 111

# 2. Configure CMake project
cmake -B "build" `
    -G "MinGW Makefiles" `
    -S "." `
    -DCMAKE_INSTALL_PREFIX="install" `
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ `
    -DCMAKE_C_STANDARD=17 -DCMAKE_CXX_STANDARD=17 `
    -DCMAKE_BUILD_TYPE=Release `
    -DQt6_DIR="third_party/qt/lib/cmake/Qt6"

# 3. Build And Install
cmake --build "build" --target install --parallel 2
