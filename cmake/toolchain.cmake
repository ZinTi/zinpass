# cmake/toolchain-base.cmake
message(STATUS "Loading base toolchain")

# 设置第三方库目录
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/../third_party")

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "${THIRD_PARTY_DIR}/lib/grpc"      # grpc 中内置了 BoringSSL，与自编译 OpenSSL 冲突
        "${THIRD_PARTY_DIR}/lib/openssl"

        "${THIRD_PARTY_DIR}/boost"
        "${THIRD_PARTY_DIR}/lib/sqlite"
        "${THIRD_PARTY_DIR}/lib/spdlog"
        "${THIRD_PARTY_DIR}/lib/yaml-cpp"
        # "/home/zin/dev/lib/qt6/qt-release-6.11.1-20260304-linux-x86_64-gcc/"
        # "${THIRD_PARTY_DIR}/lib/qt"
        # "${THIRD_PARTY_DIR}/lib/qt/lib/cmake/Qt6"
        # 本机库搜索路径
)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
