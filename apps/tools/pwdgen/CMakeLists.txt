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
add_executable(${PROJECT_NAME}
    src/pwdgen.c
    src/color.c
)

# 设置包含路径
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# 配置安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "client/bin"
)
