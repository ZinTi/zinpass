# cmake/toolchain-base.cmake
message(STATUS "Loading base toolchain")

# 设置第三方库目录
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/../third_party")

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "${THIRD_PARTY_DIR}/third_party/grpc"
        "${THIRD_PARTY_DIR}/third_party/openssl"   # 若 gRPC 库中内置的 OpenSSL冲突了，则注释掉此行
        "${THIRD_PARTY_DIR}/third_party/boost"
        "${THIRD_PARTY_DIR}/third_party/sqlite"
        "${THIRD_PARTY_DIR}/third_party/yaml-cpp"
        "${THIRD_PARTY_DIR}/third_party/qt"
        "${THIRD_PARTY_DIR}/third_party/qt/lib/cmake/Qt6"
        # 本机库搜索路径
        # "A:/applib/grpc-1.71.0-amd64-win-mingw64"
        # "A:/applib/openssl-v3.5.4-20251021-windows-x86_64-msys2"
        # "A:/applib/boost-v1.90.0-20260226-windows-x86_64-msys2/include/boost-1_90"
        # "A:/applib/sqlite-v3.50.4-20251021-windows-x86_64-mingw"
        # "A:/applib/qt-release-6.10.0-20251025-windows-x86_64-mingw"
        # "A:/applib/qt-release-6.10.0-20251025-windows-x86_64-mingw/lib/cmake/Qt6"
)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
