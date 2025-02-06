cmake_minimum_required(VERSION 3.20)

# 检查是否从顶级项目继承版本号，否则设置默认值
if(NOT DEFINED ZINPASS_VERSION)
    set(ZINPASS_VERSION 0.0.1)  # 独立构建时的默认值
endif()
project(zinpassctl VERSION ${ZINPASS_VERSION} LANGUAGES CXX)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY server/bin)

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "${PROJECT_ROOT}/third_party/grpc"
)

# 在 find_package 前强制设置静态库查找顺序
if(MINGW)
    # MinGW 下，设置 .a 优先于 .dll.a
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a .dll.a .so .lib .dylib)
elseif(WIN32 AND MSVC)
    # MSVC 下，.lib 优先于 .dll.lib
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll.lib .a .so .dylib)
else()
    # Linux/Unix 下，.a 优先于 .so/.dylib
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so .dylib .lib)
endif()

# 查找依赖包
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

if(MSVC)
    add_compile_options(/std:c++17)
endif()

# 设置变量，包含项目文件
set(PROJECT_SOURCES
        # 0. 主函数
        src/main.cpp

        # 1. 命令行交互-cli
        src/cli/prompt.cpp
        src/cli/read_eval_print_loop.cpp
        src/cli/command_handler.cpp
        src/cli/cli_command.cpp

        # 2. 通信-grpc
        src/rpc/status_rpc.cpp

        # 3. 状态-state
        src/state/channel_manager.cpp

        # auto_generated
        auto_generated/common/v1/common.pb.cc
        auto_generated/common/v1/common.grpc.pb.cc
        auto_generated/control/status.pb.cc
        auto_generated/control/status.grpc.pb.cc
        auto_generated/control/status.grpc.pb.h
)

# 添加可执行文件
add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

# 设置基本目标属性
# set_target_properties(${PROJECT_NAME} PROPERTIES
#         OUTPUT_NAME "zinpassctl"    # 输出文件名
# )

# 根据编译器设置链接选项
if(MSVC)
    # MSVC: 使用/MT静态链接运行时库，/SUBSYSTEM:WINDOWS
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/MT /SUBSYSTEM:WINDOWS /DEBUG:NONE /OPT:REF>
            $<$<CONFIG:Debug>:/MTd /SUBSYSTEM:WINDOWS>
    )

elseif(MINGW OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # MinGW/GCC: 使用-static -s
    target_link_options(${PROJECT_NAME} PRIVATE
            -static
            -s
            -Wl,--gc-sections
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Clang: 使用相应的小体积选项
    target_link_options(${PROJECT_NAME} PRIVATE
            -Wl,--gc-sections
    )
endif()

# Release模式下的额外优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/O2>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O2>
    )
endif()

# 包含头文件目录（在add_executable、add_library之后）
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include

        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated
        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/common
        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/common/v1
        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/control
        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/hello
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
        gRPC::grpc++
        gRPC::grpc
        gRPC::gpr
        protobuf::libprotobuf
)

# 安装 CLI 工具到 <prefix>/${SERVER_INSTALL_DIR}/ 目录下（与服务端放在一起）
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${SERVER_INSTALL_DIR}/bin"                # 可执行文件 -> <prefix>/${SERVER_INSTALL_DIR}/bin/
        BUNDLE DESTINATION "${SERVER_INSTALL_DIR}"                     # macOS Bundle (若需要)
        LIBRARY DESTINATION "${SERVER_INSTALL_DIR}/lib"                # 共享库
        ARCHIVE DESTINATION "${SERVER_INSTALL_DIR}/lib"                # 静态库
        RESOURCE DESTINATION "${SERVER_INSTALL_DIR}/resources"         # 资源文件
)
