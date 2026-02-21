cmake_minimum_required(VERSION 3.20)
project(ZinPass VERSION 0.0.1)  # 版本号

# 1. 定义版本变量和路径变量
set(ZINPASS_VERSION ${PROJECT_VERSION}) # 同一版本号变量（所有子项目共享）
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# 2. 定义输出目录
# 使用UTC时间
string(TIMESTAMP UTC_DATE "%Y%m%d" UTC)

# 设置全局输出路径
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置全局<prefix>
# set(CMAKE_INSTALL_PREFIX install)

# 设置平台信息，定义子项目安装目录变量
# 项目名称-v版本号-UTC日期-操作系统-处理器架构-编译器（全小写）
string(TOLOWER
    "${PROJECT_NAME}-client-v${ZINPASS_VERSION}-${UTC_DATE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_C_COMPILER_ID}" CLIENT_INSTALL_DIR
)
string(TOLOWER
    "${PROJECT_NAME}-server-v${ZINPASS_VERSION}-${UTC_DATE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_C_COMPILER_ID}" SERVER_INSTALL_DIR
)

# 定义子项目的构建选项
option(BUILD_SERVICE "构建服务端（zinpasssvc/zinpassd）" ON)
option(BUILD_CONTROL "构建控制台工具（zinpassctl）" ON)
option(BUILD_CLIENT_DESKTOP "构建客户端桌面应用" ON)
# option(BUILD_TOOLS_PWDGEN "构建密码生成器工具" OFF) # 示例：默认不构建的工具

# 引入子项目
if(BUILD_SERVICE)
    add_subdirectory(service)
endif()
if(BUILD_CONTROL)
    add_subdirectory(control)
endif()
if(BUILD_CLIENT_DESKTOP)
    add_subdirectory(client_desktop)
endif()
# if(BUILD_TOOLS_PWDGEN)
#     add_subdirectory(tools/pwdgen)
# endif()

# 配置全局安装规则（客户端和服务端各自一份）
install(FILES README.md LICENSE DESTINATION ${CLIENT_INSTALL_DIR})
install(FILES README.md LICENSE DESTINATION ${SERVER_INSTALL_DIR})
