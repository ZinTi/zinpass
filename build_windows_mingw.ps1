# Build Zinpass for Windows using MinGW-w64

# Configure CMake project
cmake -B "build" `
    -G "MinGW Makefiles" `
    -S "." `
    -DCMAKE_INSTALL_PREFIX="install" `
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ `
    -DCMAKE_C_STANDARD=17 -DCMAKE_CXX_STANDARD=17 `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain.cmake" `
    -DCMAKE_INSTALL_RPATH='$ORIGIN:$ORIGIN/../lib'

# Build And Install
cmake --build "build" --target install --parallel 12
