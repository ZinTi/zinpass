# cmake/toolchain-base.cmake
message(STATUS "Loading base toolchain")

# 设置第三方库目录
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/../third_party")

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "${THIRD_PARTY_DIR}/grpc"
        "${THIRD_PARTY_DIR}/openssl"   # 若 gRPC 库中内置的 OpenSSL冲突了，则注释掉此行
        "${THIRD_PARTY_DIR}/boost"
        "${THIRD_PARTY_DIR}/sqlite"
        "${THIRD_PARTY_DIR}/yaml-cpp"
        "${THIRD_PARTY_DIR}/qt"
        "${THIRD_PARTY_DIR}/qt/lib/cmake/Qt6"
        # 本机库搜索路径
)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
