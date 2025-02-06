cmake_minimum_required(VERSION 3.20)
project(pwdgen LANGUAGES C)

# 设置默认构建类型为 Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 设置优化选项
set(OPTIMIZATION_FLAGS "-Os -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${OPTIMIZATION_FLAGS}")

# 添加可执行文件
add_executable(pwdgen
    src/pwdgen.c
    src/color.c
)

# 设置包含路径
target_include_directories(pwdgen
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# 安装规则
install(TARGETS pwdgen
    RUNTIME DESTINATION bin
)