#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <mutex>
#include <string>

namespace zinpass::config {

class StateManager {
public:
    static StateManager& instance();
    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    /**
     * 获取工作根路径
     */
    std::string get_work_root_path() const;

    /**
     * 设置工作根路径
     */
    void set_work_root_path(const std::string& path);

    /**
     * 获取服务器地址端口配置
     */
    std::string get_server_address() const;

    /**
     * 设置服务器地址端口配置
     */
    void set_server_address(const std::string &address = "[::1]:50051");

    /**
     * @brief 获取主数据库路径配置
     * @return 主数据库文件路径
     */
    std::string get_main_database_path() const;

    /**
     * @brief 设置主数据库路径配置
     * @param path 主数据库路径
     */
    void set_main_database_path(const std::string &path);

    /**
     * @brief 获取运行日志文件路径配置
     * @return 运行日志文件路径
     */
    std::string get_run_log_path() const;

    /**
     * @brief 设置运行日志文件路径配置
     * @param path 运行日志文件路径
     */
    void set_run_log_path(const std::string &path);

    /**
     * 获取当前进程可执行文件路径
     * @return 路径
     */
    std::string get_executable_path();

private:
    StateManager() = default;

    mutable std::mutex config_mutex_;
    std::string work_root_path_;            // 服务端工作根路径
    std::string server_address_;            // 监听地址+端口

    std::string main_database_path_;        // 主数据库路径
    std::string run_log_path_;              // 运行日志路径
};

} // zinpass::config

#endif //STATE_MANAGER_H
