# CMake 构建 SQLite3 静态库（不含 SQLite Shell 可执行文件）
cmake_minimum_required(VERSION 3.20)
project(sqlite3 VERSION 3.53.3)  # 版本号根据实际 sqlite3.h 中的定义调整

# --------------------------------------------------------------
# 1. 构建 SQLite3 静态库
# --------------------------------------------------------------
add_library(sqlite3 STATIC sqlite3.c)

# 启用位置无关代码（便于静态库被链接到共享库或可执行文件）
set_target_properties(sqlite3 PROPERTIES POSITION_INDEPENDENT_CODE ON)

# 公开头文件目录（构建时使用当前源码目录，安装时使用 include）
target_include_directories(sqlite3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 常用的 SQLite 编译宏（可根据需要增减）
target_compile_definitions(sqlite3 PUBLIC
    SQLITE_THREADSAFE=1          # 启用线程安全
    SQLITE_ENABLE_COLUMN_METADATA
    # 若想禁用扩展加载，可添加: SQLITE_OMIT_LOAD_EXTENSION
)

# 针对 Windows（MSVC/MinGW）不导出动态库符号（静态库无需 __declspec(dllexport)）
if(WIN32)
    target_compile_definitions(sqlite3 PRIVATE SQLITE_API=)
endif()

# 创建别名目标，便于上层统一使用
add_library(SQLite3::SQLite3 ALIAS sqlite3)

# --------------------------------------------------------------
# 2. 安装规则（供上层项目 install 使用）
# --------------------------------------------------------------
install(TARGETS sqlite3
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(FILES sqlite3.h sqlite3ext.h DESTINATION include)
