cmake_minimum_required(VERSION 3.20)
project(ZinPassProject VERSION 0.0.1)  # 版本号

# 1. 定义统一版本号变量（所有子项目共享）
set(ZINPASS_VERSION ${PROJECT_VERSION})

# 2. 定义输出目录

set(PROGRAM_NAME "zinpass")

if(WIN32)
    set(OS_NAME "windows")
else()
    set(OS_NAME "linux")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ARCH_NAME "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARCH_NAME "aarch64")
else()
    message(WARNING "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    set(ARCH_NAME "unknown")
endif()

if(WIN32)
    set(TOOLCHAIN_NAME "mingw")
else()
    set(TOOLCHAIN_NAME "gcc")
endif()

# 设置所有可执行文件的输出目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/zinpass-v${ZINPASS_VERSION}-${OS_NAME}-x86_64-gcc/bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROGRAM_NAME}-${COMPONENT_NAME}-v${ZINPASS_VERSION}-${OS_NAME}-${ARCH_NAME}-${TOOLCHAIN_NAME}/bin)

# 引入子项目
add_subdirectory(service)
add_subdirectory(control)
add_subdirectory(client_desktop)
# add_subdirectory(tools/pwdgen)
