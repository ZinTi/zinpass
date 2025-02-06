#ifndef UTIL_EXECUTABLE_PATH_H
#define UTIL_EXECUTABLE_PATH_H

#include <filesystem>
#include <string>

namespace zinpass::utils {

/**
 * @brief 获取当前进程可执行文件路径
 * @return 可执行文件的完整路径
 */
std::filesystem::path get_executable_path();

/**
 * @brief 获取可执行文件的父目录的父目录（安装目录根目录）
 * @return 可执行文件的父目录的父目录路径
 */
std::filesystem::path get_executable_grandparent_dir();

}  // namespace zinpass::utils

#endif  // UTIL_EXECUTABLE_PATH_H
