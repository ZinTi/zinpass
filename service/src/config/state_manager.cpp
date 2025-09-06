#include "config/state_manager.h"
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace zinpass::config {

    StateManager& StateManager::instance() {
        static StateManager instance;
        return instance;
    }

    std::string StateManager::get_work_root_path() const {
        std::lock_guard<std::mutex> lock(config_mutex_);
        return work_root_path_;
    }

    void StateManager::set_work_root_path(const std::string& path) {
        std::lock_guard<std::mutex> lock(config_mutex_);
        work_root_path_ = path;
    }

    std::string StateManager::get_server_address() const {
        std::lock_guard<std::mutex> lock(config_mutex_);
        return server_address_;
    }

    void StateManager::set_server_address(const std::string& address) {
        std::lock_guard<std::mutex> lock(config_mutex_);
        server_address_ = address;
    }

    std::string StateManager::get_main_database_path() const {
        std::lock_guard<std::mutex> lock(config_mutex_);
        return main_database_path_;
    }

    void StateManager::set_main_database_path(const std::string &path) {
        std::lock_guard<std::mutex> lock(config_mutex_);
        main_database_path_ = path;
    }

    std::string StateManager::get_run_log_path() const {
        std::lock_guard<std::mutex> lock(config_mutex_);
        return run_log_path_;
    }

    void StateManager::set_run_log_path(const std::string &path) {
        std::lock_guard<std::mutex> lock(config_mutex_);
        run_log_path_ = path;
    }

    std::string StateManager::get_executable_path() {
#ifdef _WIN32
        // Windows 动态处理长路径
        std::vector<char> path(MAX_PATH);
        DWORD length = 0;

        while (true) {
            length = GetModuleFileNameA(nullptr, path.data(), path.size());
            if (length < path.size()) break; // 成功获取
            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) return "";
            path.resize(path.size() * 2);    // 缓冲区翻倍
        }
        return (length != 0) ? std::string(path.data()) : "";

#else
        // Linux/Unix 动态分配内存
        std::vector<char> path(1024);
        ssize_t count = -1;

        while (true) {
            count = readlink("/proc/self/exe", path.data(), path.size());
            if (count < 0) return "";         // 读取失败
            if (count < path.size()) break;    // 缓冲区足够
            path.resize(path.size() * 2);      // 缓冲区翻倍
        }
        return std::string(path.data(), count);
#endif
    }

}

