cmake_minimum_required(VERSION 3.20)

# 检查是否从顶级项目继承版本号，否则设置默认值
if(NOT DEFINED ZINPASS_VERSION)
    set(ZINPASS_VERSION 0.0.1)  # 独立构建时的默认值
endif()
project(client_desktop VERSION ${ZINPASS_VERSION} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY client/bin)

# 设置平台相关变量
if(WIN32)
    set(EXECUTABLE_NAME "zinpass")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(EXECUTABLE_NAME "zinpass")
else()
    set(EXECUTABLE_NAME "zinpass")
endif()

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "${PROJECT_ROOT}/third_party/qt"
        "${PROJECT_ROOT}/third_party/qt/lib/cmake/Qt6"
        "${PROJECT_ROOT}/third_party/grpc"
)

# 设置 gRPC 和 Protobuf 的根目录
set(GRPC_INSTALL_DIR "${PROJECT_ROOT}/third_party/grpc")
set(Protobuf_ROOT ${GRPC_INSTALL_DIR})
set(gRPC_ROOT ${GRPC_INSTALL_DIR})

# 设置 Protobuf 的 CMake 配置路径
set(Protobuf_DIR "${GRPC_INSTALL_DIR}/lib/cmake/protobuf")

# 添加自定义 CMake 模块路径（包含 grpc 的 Config.cmake）
list(APPEND CMAKE_MODULE_PATH
        "${GRPC_INSTALL_DIR}/cmake"
)

# 启用自动处理moc/uic/rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/qt/ui) # 创建 ui_*.h 生成目录
#set(CMAKE_AUTOUIC_OPTIONS "-o ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/qt/ui") # 全局设置uic输出目录

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

# 查找 Qt、Protobuf、 gRPC
set(QT_NO_PRIVATE_MODULE_WARNING ON)  # 抑制私有模块警告
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
# find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 对OpenGL和X11的查找（Linux平台）
if(UNIX AND NOT APPLE)  # Linux平台
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
endif()

# 包含头文件目录
include_directories(
        ${OPENSSL_INCLUDE_DIR} # 添加 OpenSSL 头文件路径
        ${SQLite3_INCLUDE_DIR} # 添加 SQLite3 头文件路径
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui

        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated
        ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/grpc
        # ${CMAKE_CURRENT_SOURCE_DIR}/auto_generated/qt/ui
)

set(PROJECT_SOURCES
        # 1. 主函数
        src/main.cpp

        # 2. 应用运行时状态管理层（全局变量、跨组件的共享状态、网络请求的临时上下文、当前用户对象、UI主题状态、临时缓存） state
        src/state/state_manager.cpp
        src/state/channel_manager.cpp
        src/state/client_config.cpp

        # 3. 本地持久化存储抽象层（用户偏好设置、本地缓存、加密的会话令牌、设备标识等）storage
        # 暂无

        # 4. 用户侧模型 Models
        src/models/user.cpp
        src/models/session.cpp
        src/models/mobile_phone.cpp
        src/models/account.cpp
        src/models/view_account.cpp

        # 5. Utils 层
        src/utils/date_time.cpp
        # src/utils/pwd_utils.cpp
        src/utils/pwd_generator.cpp
        src/utils/uuid_generator.cpp
        src/utils/executable_path.cpp

        # 6. UI 层
        # (6.0) 资源文件
        res/resource.qrc
        res/icon_zinpass.rc # windows平台pe文件图标
        # (6.1) 常用模块与公共组件
        src/ui/common/font_util.cpp
        src/ui/common/toast.cpp
        src/ui/common/numbered_text_edit.cpp
        src/ui/common/search_box.cpp
        src/ui/common/dlg_auth.cpp    # 验证当前用户身份
        src/ui/common/is_input_valid.cpp # 判断输入的数据是否合法-前端部分辅助验证

        # (6.2) 系统登录界面、系统用户注册界面
        src/ui/user/login_dlg.cpp
        src/ui/user/signup_dlg.cpp
        src/ui/user/personal_mgr_form.cpp
        src/ui/user/preferences_form.cpp
        src/ui/user/settings_dlg.cpp
        # (6.3) 主工作台
        src/ui/main_workbench.cpp
        # (6.4) 主功能菜单
        src/ui/function_menu_form.cpp
        # (6.5) 管理账号：页面框架、添加和修改账号记录、修改账号所属人
        src/ui/account/account_filter_form.cpp
        src/ui/account/account_mgr_form.cpp
        src/ui/account/popup_filter_window.cpp
        src/ui/account/dlg_exposed_pwd.cpp # 查看明文密码的弹窗
        src/ui/account/dlg_add_account.cpp
        src/ui/account/dlg_edit_account.cpp
        src/ui/account/dlg_delete_account.cpp
        src/ui/account/data_owner_form.cpp
        src/ui/account/check_pwd_security_form.cpp # 安全检查页面

        # (6.6) 管理手机号：页面框架、增加手机号
        src/ui/phone/phone_filter_form.cpp
        src/ui/phone/phone_detail_frame.cpp
        src/ui/phone/dlg_add_phone.cpp
        src/ui/phone/phone_mgr_form.cpp
        src/ui/phone/deletion_guide_dlg.cpp

        # (6.7) 工具类：密码生成器、密码安全检查器、加密解密器
        src/ui/tools/pwd_gen_dlg.cpp
        src/ui/tools/uuid_gen_dlg.cpp
        src/ui/tools/notepad.cpp

        # (6.8) 迁移备份数据
        src/ui/output_form.cpp
        src/ui/input_form.cpp
        # (6.9) 日志管理
        src/ui/log_run_form.cpp
        src/ui/log_handle_form.cpp

        # 7、RPC 层
        # (7.1) 接口
        src/rpc/user_rpc.cpp
        src/rpc/auth_rpc.cpp
        src/rpc/telecom_rpc.cpp
        src/rpc/phone_rpc.cpp
        src/rpc/account_rpc.cpp
        src/rpc/category_rpc.cpp
        # (7.2) auto_generated
        auto_generated/grpc/common/v1/common.pb.cc
        auto_generated/grpc/common/v1/common.grpc.pb.cc
        auto_generated/grpc/user/v1/user.pb.cc
        auto_generated/grpc/user/v1/user.grpc.pb.cc
        auto_generated/grpc/user/v1/auth.pb.cc
        auto_generated/grpc/user/v1/auth.grpc.pb.cc
        auto_generated/grpc/account/v1/telecom.pb.cc
        auto_generated/grpc/account/v1/telecom.grpc.pb.cc
        auto_generated/grpc/account/v1/phone.pb.cc
        auto_generated/grpc/account/v1/phone.grpc.pb.cc
        auto_generated/grpc/account/v1/account.pb.cc
        auto_generated/grpc/account/v1/account.grpc.pb.cc
        auto_generated/grpc/account/v1/category.pb.cc
        auto_generated/grpc/account/v1/category.grpc.pb.cc
)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

# 链接 Qt6 和 OpenSSL、Windows 系统库
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets

        # gRPC相关库
        gRPC::grpc++
        gRPC::grpc
        #gRPC::grpc_cronet
        gRPC::gpr
        protobuf::libprotobuf

        # yaml-cpp 库
        yaml-cpp::yaml-cpp

        # Windows专用库（仅Windows平台链接）
        $<$<PLATFORM_ID:Windows>:ws2_32>
        $<$<PLATFORM_ID:Windows>:user32>
        $<$<PLATFORM_ID:Windows>:advapi32>
        $<$<PLATFORM_ID:Windows>:crypt32>
)

# Linux专用库
if(UNIX AND NOT APPLE)
    # 使用 pkg-config 简化依赖管理
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLIB2 glib-2.0 REQUIRED)
    pkg_check_modules(GIO2 gio-2.0 REQUIRED)
    pkg_check_modules(GIO_UNIX gio-unix-2.0 REQUIRED)
    pkg_check_modules(XCB xcb REQUIRED)
    pkg_check_modules(X11_XCB x11-xcb REQUIRED)
    pkg_check_modules(SYSTEMD libsystemd REQUIRED)
    
    # 添加 Qt 静态库所依赖的其他缺失的库
    # 注意：静态链接时顺序很重要，依赖的库放在后面
    target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        X11::X11
        ${XCB_LIBRARIES}
        ${X11_XCB_LIBRARIES}
        ${GLIB2_LIBRARIES}
        ${GIO2_LIBRARIES}
        ${GIO_UNIX_LIBRARIES}
        ${SYSTEMD_LIBRARIES}

        # 将所有有依赖关系的库放在 --start-group 和 --end-group 之间
        -Wl,--start-group
        -lgio-2.0
        -lgmodule-2.0
        -lgobject-2.0
        -lglib-2.0
        -lffi
        -lpcre2-8
        -lX11-xcb
        -lxcb
        -lXau
        -lXdmcp
        -lsystemd
        -lmount
        -lselinux
        -ldbus-1
        -Wl,--end-group

        -pthread
        -ldl
        -lm
        -lrt
    )
    
    # 添加包含目录
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${GIO2_INCLUDE_DIRS}
        ${XCB_INCLUDE_DIRS}
        ${SYSTEMD_INCLUDE_DIRS}
    )
endif()

# 设置基本目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE                           # Win32 应用
        OUTPUT_NAME "${EXECUTABLE_NAME}"                # 设置输出文件名
)

# 根据编译器设置链接选项
if(MSVC)
    # MSVC: 使用/MT静态链接运行时库，/SUBSYSTEM:WINDOWS
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/MT /SUBSYSTEM:WINDOWS /DEBUG:NONE /OPT:REF>
            $<$<CONFIG:Debug>:/MTd /SUBSYSTEM:WINDOWS>
    )

elseif(MINGW)
    # MinGW: 使用-static -s
    target_link_options(${PROJECT_NAME} PRIVATE
            -static
            -s
            -Wl,--gc-sections
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # Linux下：不要使用-static，避免静态链接动态库
    # 但可以尝试使用-Wl,-Bstatic和-Wl,-Bdynamic来控制链接方式
    target_link_options(${PROJECT_NAME} PRIVATE
            -s
            -Wl,--gc-sections
            # 使用链接器选项移除对不需要库的引用
            -Wl,--as-needed
            # 尝试先静态链接，如果失败则动态链接
            # -Wl,-Bstatic -lGL -Wl,-Bdynamic
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Linux下：不要使用-static，避免静态链接动态库
    # 但可以尝试使用-Wl,-Bstatic和-Wl,-Bdynamic来控制链接方式
    # Clang: 使用相应的小体积选项
    target_link_options(${PROJECT_NAME} PRIVATE
            -s
            -Wl,--gc-sections
            # 使用链接器选项移除对不需要库的引用
            -Wl,--as-needed
            # 尝试先静态链接，如果失败则动态链接
            # -Wl,-Bstatic -lGL -Wl,-Bdynamic
    )
endif()

# Release模式下的额外优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/O2>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O2>
    )
endif()

# 静态编译专用配置
target_compile_definitions(${PROJECT_NAME} PRIVATE
        QT_STATICPLUGIN  # 启用静态插件支持
)

# 配置安装规则（安装客户端到 <prefix>/${CLIENT_INSTALL_DIR}/ 目录下）
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CLIENT_INSTALL_DIR}/bin"                # 可执行文件
        BUNDLE DESTINATION "${CLIENT_INSTALL_DIR}"                     # macOS Bundle (若需要)
        LIBRARY DESTINATION "${CLIENT_INSTALL_DIR}/lib"                # 共享库
        ARCHIVE DESTINATION "${CLIENT_INSTALL_DIR}/lib"                # 静态库
        RESOURCE DESTINATION "${CLIENT_INSTALL_DIR}/resources"         # 资源文件
)

# 安装复制 shared libraries 脚本
install(PROGRAMS "scripts/copy_shared_lib.sh" DESTINATION "${CLIENT_INSTALL_DIR}/bin" )

# 安装客户端的配置文件
install(FILES "config/client_config.example.yml" DESTINATION "${CLIENT_INSTALL_DIR}/bin")

# # 切换到安装好的目录 "${CLIENT_INSTALL_DIR}/bin" 下执行脚本 "./copy_shared_lib.sh"
# install(CODE [[
#     # 在目标目录执行脚本
#     execute_process(
#         COMMAND ./copy_shared_lib.sh
#         WORKING_DIRECTORY "${CLIENT_INSTALL_DIR}/bin" 
#     )
# ]])
