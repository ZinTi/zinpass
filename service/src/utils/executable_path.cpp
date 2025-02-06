#include "utils/executable_path.h"

#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

// 平台特定头文件
#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#elif defined(__linux__)
    #include <unistd.h>
#else
#error "Unsupported platform"
#endif

namespace zinpass::utils {

/**
 * @brief 获取当前可执行文件的完整路径
 * 
 * 该函数通过平台特定的API获取当前运行的可执行文件的完整路径。
 * 在Windows上使用GetModuleFileNameA，在macOS上使用_NSGetExecutablePath，
 * 在Linux上通过读取/proc/self/exe符号链接实现。
 * 
 * @return std::filesystem::path 可执行文件的完整路径
 * @throw std::system_error 当平台API调用失败时抛出，包含系统错误码
 * @throw std::runtime_error 当其他错误发生时抛出
 */
std::filesystem::path get_executable_path() {
    std::string path_string;

#if defined(_WIN32)
    // Windows实现
    DWORD buffer_size = MAX_PATH; // 初始缓冲区大小
    std::vector<char> buffer(buffer_size);

    while (true) {
        DWORD length = GetModuleFileNameA(nullptr, buffer.data(), buffer_size);
        if (length == 0) {
            DWORD error_code = GetLastError();
            throw std::system_error(
                static_cast<int>(error_code),
                std::system_category(),
                "GetModuleFileNameA failed"
            );
        }
        // 如果复制的字符数小于缓冲区大小减一，说明缓冲区足够
        if (length < buffer_size - 1) {
            path_string.assign(buffer.data(), length);
            break;
        }
        // 缓冲区可能不足，扩大后重试
        buffer_size *= 2;
        buffer.resize(buffer_size);
    }

#elif defined(__APPLE__)
    // macOS实现
    uint32_t size = 0;
    // 第一次调用获取所需缓冲区大小
    if (_NSGetExecutablePath(nullptr, &size) != -1) {
        // 如果返回0，表示缓冲区不足，但size已被设置
        throw std::runtime_error("_NSGetExecutablePath failed");
    }
    std::vector<char> buffer(size);
    if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
        throw std::runtime_error("_NSGetExecutablePath failed");
    }
    path_string.assign(buffer.data());

#elif defined(__linux__)
    // Linux实现（也适用于其他支持/proc/self/exe的POSIX系统）
    std::vector<char> buffer(1024); // 初始缓冲区大小
    ssize_t length = 0;

    while (true) {
        length = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
        if (length == -1) {
            int error_code = errno;
            throw std::system_error(
                error_code,
                std::system_category(),
                "readlink /proc/self/exe failed"
            );
        }
        // 如果读取的字符数小于缓冲区大小减一，说明缓冲区足够
        if (static_cast<size_t>(length) < buffer.size() - 1) {
            buffer[length] = '\0'; // 手动添加终止符
            path_string.assign(buffer.data());
            break;
        }
        // 缓冲区不足，扩大后重试
        buffer.resize(buffer.size() * 2);
    }

#else
    // 不支持的平台已在编译时通过#error处理
#endif

    return std::filesystem::path(path_string);
}

/**
 * @brief 获取可执行文件祖父目录的路径
 * 
 * 该函数返回可执行文件所在目录的父目录。例如，如果可执行文件路径为
 * /opt/zinpass/bin/app，则返回/opt/zinpass。
 * 
 * @return std::filesystem::path 可执行文件祖父目录的路径
 */
std::filesystem::path get_executable_grandparent_dir() {
    // 1. 获取当前可执行文件的完整路径
    std::filesystem::path executable_path = get_executable_path();
    // 2. 移除文件名，获取其所在目录
    std::filesystem::path bin_dir = executable_path.parent_path();
    // 3. 获取该目录的父目录
    std::filesystem::path parent_dir = bin_dir.parent_path();
    return parent_dir;
}

} // namespace zinpass::utils
