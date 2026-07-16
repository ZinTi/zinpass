cmake_minimum_required(VERSION 3.20)
project(ZinPass VERSION 0.0.2)  # 版本号

# 1. 定义版本变量和路径变量
set(ZINPASS_VERSION ${PROJECT_VERSION}) # 同一版本号变量（所有子项目共享）
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# 将版本号定义传递给编译器
add_definitions(-DZINPASS_VERSION="${ZINPASS_VERSION}")

# 2. 下载并构建第三方库源代码
# include(FetchContent)

# option(USE_GITHUB_YAMLCPP "是否使用GITHUB的YAMLCPP" ON)

# if(USE_GITHUB_YAMLCPP)
#     FetchContent_Declare(
#     yaml-cpp
#     GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#     GIT_TAG yaml-cpp-0.9.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
#     )
#     FetchContent_MakeAvailable(yaml-cpp)
# else()
#     find_package(yaml-cpp REQUIRED)
# endif()

# 定义统一的输出目录
# 使用UTC时间
string(TIMESTAMP UTC_DATE "%Y%m%d" UTC)

# 设置全局输出路径
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置全局<prefix>
# set(CMAKE_INSTALL_PREFIX install)

# platform info: 项目名称-v版本号-UTC日期-操作系统-处理器架构-编译器 (lowercase)
string(TOLOWER
        "${PROJECT_NAME}-v${ZINPASS_VERSION}-${UTC_DATE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_C_COMPILER_ID}"
        ZINPASS_INSTALL_DIR
)


# # ---------- 第三方库构建设置 ----------
# # 强制构建静态库（除非明确需要共享库）
# set(BUILD_SHARED_LIBS OFF)

# # 统一输出目录（静态库 .a / .lib 会放在 build/lib 下）
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# # 添加第三方库源码（按依赖顺序）
# add_subdirectory(third_party/boost)        # 仅头文件
# add_subdirectory(third_party/spdlog)
# add_subdirectory(third_party/yaml-cpp)
# add_subdirectory(third_party/sqlite)       # 手动编写的 CMakeLists.txt
# # add_subdirectory(third_party/openssl)      # 没有官方 CMakeLists.txt
# add_subdirectory(third_party/grpc)         # grpc 依赖 protobuf（其在 grpc/third_party 内）

# ---------- 添加应用程序子项目 ----------
# 定义子项目的构建选项
option(BUILD_SERVER "构建服务端 zinpassd" ON)
option(BUILD_CONTROL "构建控制台工具 zinpassctl" ON)
option(BUILD_CLIENT_DESKTOP "构建客户端桌面应用 zinpass" ON)
# option(BUILD_TOOLS_PWDGEN "构建密码生成器工具" OFF) # 示例：默认不构建的工具

# 引入子项目
if(BUILD_SERVER)
    add_subdirectory(apps/server)
endif()
if(BUILD_CONTROL)
    add_subdirectory(apps/server_ctl)
endif()
if(BUILD_CLIENT_DESKTOP)
    add_subdirectory(apps/client_desktop)
endif()
# if(BUILD_TOOLS_PWDGEN)
#     add_subdirectory(apps/tools/pwdgen)
# endif()

# 配置全局安装规则（合并安装）
install(FILES README.md LICENSE DESTINATION ${ZINPASS_INSTALL_DIR})
