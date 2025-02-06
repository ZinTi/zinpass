cmake_minimum_required(VERSION 3.20)

# 检查是否从顶级项目继承版本号，否则设置默认值
if(NOT DEFINED ZINPASS_VERSION)
    set(ZINPASS_VERSION 0.0.1)  # 独立构建时的默认值
endif()
project(zinpass_service VERSION ${ZINPASS_VERSION} LANGUAGES CXX)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY server/bin)

# 设置平台相关变量
if(WIN32)
    set(EXECUTABLE_NAME "zinpassd")    # Windows 下生成 zinpassd.exe
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(EXECUTABLE_NAME "zinpassd")      # Linux 下生成 zinpassd
else()
    set(EXECUTABLE_NAME "zinpassd") # 其他平台
endif()

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "${PROJECT_ROOT}/third_party/grpc"
        "${PROJECT_ROOT}/third_party/openssl"   # 若 gRPC 库中内置的 OpenSSL冲突了，则注释掉此行
        "${PROJECT_ROOT}/third_party/boost"
        "${PROJECT_ROOT}/third_party/sqlite"
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
find_package(OpenSSL REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(SQLite3 REQUIRED)

# # 下载并构建第三方库源代码
# include(FetchContent)

# FetchContent_Declare(
#   yaml-cpp
#   GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#   GIT_TAG yaml-cpp-0.9.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
# )
# FetchContent_MakeAvailable(yaml-cpp)

# 平台特定源文件
if(WIN32)
    set(PLATFORM_SOURCES
        src/main_windows.cpp
    )
else()
    set(PLATFORM_SOURCES
        src/main_linux.cpp
    )
endif()

# 公共源文件
set(COMMON_SOURCES
        # 0. 应用程序
        src/application.cpp

        # 1. 通用工具-utils
        src/utils/tmp_msg.cpp
        src/utils/date_time.cpp
        src/utils/pwd_utils.cpp
        src/utils/pwd_generator.cpp
        src/utils/uuid_generator.cpp
        src/utils/log_manager.cpp
        src/utils/executable_path.cpp

        # 2. 服务配置-config
        src/config/about.cpp
        src/config/app_config.cpp

        # 3. 业务逻辑层-mgr
        src/mgr/user_mgr.cpp
        src/mod_session/session_persistence.cpp
        src/mod_session/session_mgr.cpp
        src/mgr/telecom_operator_mgr.cpp
        src/mgr/phone_mgr.cpp
        src/mgr/account_mgr.cpp
        src/mgr/authentication.cpp

        # 4. 数据访问层-repo
        src/repo/connection_pool.cpp
        src/repo/common/sql_debug.cpp
        src/repo/init_database.cpp # 初始化 Database
        src/repo/user_dao.cpp
        src/mod_session/session_dao.cpp
        src/repo/telecom_operator_dao.cpp
        src/repo/phone_dao.cpp
        src/repo/category_dao.cpp
        src/repo/account_dao.cpp

        # 5. 领域模型（与proto解耦）-models
        src/models/system_user.cpp
        src/mod_session/session.cpp
        src/models/mobile_phone.cpp
        src/models/account.cpp
        src/models/view_account.cpp

        # 6. gRPC 服务实现层-rpc
        src/rpc/grpc_client_service.cpp
        src/rpc/grpc_control_service.cpp
        src/rpc/hello_service.cpp       # 测试 RPC 是否连通
        src/rpc/user/user_service.cpp
        src/rpc/user/auth_service.cpp
        src/rpc/phone/telecom_service.cpp
        src/rpc/phone/phone_service.cpp
        src/rpc/account/account_service.cpp
        src/rpc/account/category_service.cpp
        src/rpc/status/status_service.cpp

        # auto_generated
        auto_generated/common/v1/common.pb.cc
        auto_generated/common/v1/common.grpc.pb.cc
        auto_generated/hello/hello.pb.cc
        auto_generated/hello/hello.grpc.pb.cc
        auto_generated/control/status.pb.cc
        auto_generated/control/status.grpc.pb.cc

        auto_generated/user/v1/user.pb.cc
        auto_generated/user/v1/user.grpc.pb.cc
        auto_generated/user/v1/auth.pb.cc
        auto_generated/user/v1/auth.grpc.pb.cc
        auto_generated/account/v1/telecom.pb.cc
        auto_generated/account/v1/telecom.grpc.pb.cc
        auto_generated/account/v1/phone.pb.cc
        auto_generated/account/v1/phone.grpc.pb.cc
        auto_generated/account/v1/account.pb.cc
        auto_generated/account/v1/account.grpc.pb.cc
        auto_generated/account/v1/category.pb.cc
        auto_generated/account/v1/category.grpc.pb.cc
)

# 添加可执行文件
add_executable(${PROJECT_NAME}
        ${PLATFORM_SOURCES}
        ${COMMON_SOURCES}
)

# 设置基本目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${EXECUTABLE_NAME}"    # 输出文件名
)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/
        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/

        ${OPENSSL_INCLUDE_DIR} # OpenSSL 头文件

        # Windows 和 Linux 下 Boost 头文件路径可能不同，都添加到搜索路径中
        "${PROJECT_ROOT}/third_party/boost/include/boost-1_90"
        "${PROJECT_ROOT}/third_party/boost/include"
)

# 仅Windows平台设置无控制台
if(WIN32)
    # 判断编译器类型
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem,windows") # MinGW-w64/GCC/Clang使用 -Wl,-subsystem,windows
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS") # MSVC使用 /SUBSYSTEM:WINDOWS
    endif()
endif()

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SQLite::SQLite3

        gRPC::grpc++
        gRPC::grpc
        gRPC::gpr
        protobuf::libprotobuf

        yaml-cpp::yaml-cpp
)

# 安装服务端到 <prefix>/${SERVER_INSTALL_DIR}/ 目录下
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${SERVER_INSTALL_DIR}/bin"                # 可执行文件 -> <prefix>/${SERVER_INSTALL_DIR}/bin/
        BUNDLE DESTINATION "${SERVER_INSTALL_DIR}"                     # macOS Bundle (若需要)
        LIBRARY DESTINATION "${SERVER_INSTALL_DIR}/lib"                # 共享库
        ARCHIVE DESTINATION "${SERVER_INSTALL_DIR}/lib"                # 静态库
        RESOURCE DESTINATION "${SERVER_INSTALL_DIR}/resources"         # 资源文件
)

# 安装服务端配置文件
install(FILES "config/service_config.example.yml" DESTINATION "${SERVER_INSTALL_DIR}/bin")

# 安装服务端脚本
install(PROGRAMS "start_zinpassd.sh" "stop_zinpassd.sh" DESTINATION "${SERVER_INSTALL_DIR}/bin")

# 安装服务端的数据文件
install(DIRECTORY "data/" DESTINATION "${SERVER_INSTALL_DIR}/data")
