cmake_minimum_required(VERSION 3.20)
project(ZinPassProject VERSION 0.0.1)  # 版本号

# 定义统一版本号变量（所有子项目共享）
set(ZINPASS_VERSION ${PROJECT_VERSION})

if(WIN32)
    set(OS_NAME "win")
else()
    set(OS_NAME "linux")
endif()

# 设置所有可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/zinpass-v${ZINPASS_VERSION}-${OS_NAME}-mingw64/bin)

# 引入子项目
add_subdirectory(service)
add_subdirectory(control)
add_subdirectory(client_desktop)
add_subdirectory(tools/pwdgen)
