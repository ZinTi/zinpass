/**
 * @file app_config.h
 * @brief 应用程序配置类（单例）
 * 
 * 提供配置的读写功能，支持从 YAML 文件加载配置，并在文件不存在时生成默认配置。
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <mutex>
#include <string>
#include <filesystem>
#include <iostream>

#define RPC_DEFAULT_CLIENT_IPV4             "0.0.0.0"
#define RPC_DEFAULT_CLIENT_PORT             50051
#define RPC_DEFAULT_CTRL_IPV4               "127.0.0.1"
#define RPC_DEFAULT_CTRL_PORT               50052
#define DEFAULT_SESSION_TIMEOUT             600
#define LOG_DEFAULT_CLEANUP_INTERVAL        7 * 24 * 3600
#define LOG_DEFAULT_MAX_SIZE                10 * 1024 * 1024

namespace zinpass::config {

/**
 * @brief 应用程序配置单例类
 * 
 * 管理服务端运行时配置，包括网络监听地址、会话超时、数据库路径、日志路径等。
 * 配置默认基于可执行文件路径的父目录（安装根目录）构建。
 */
class AppConfig {
public:
    // 单例模式：禁用拷贝构造和赋值
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    /**
     * @brief 获取单例实例
     * @return AppConfig& 单例引用
     */
    static AppConfig& instance();

    // ----- setter 方法（支持默认参数）-----
    void set_client_ipv4(const std::string& ipv4 = RPC_DEFAULT_CLIENT_IPV4);
    void set_client_port(unsigned int port = RPC_DEFAULT_CLIENT_PORT);
    void set_cli_ipv4(const std::string& ipv4 = RPC_DEFAULT_CTRL_IPV4);
    void set_cli_port(unsigned int port = RPC_DEFAULT_CTRL_PORT);
    void set_session_timeout(long timeout = DEFAULT_SESSION_TIMEOUT);
    void set_db_path(const std::string& path);
    void set_db_path_bak(const std::string& path);
    void set_log_path(const std::string& path);
    void set_log_cleanup_interval(long interval = LOG_DEFAULT_CLEANUP_INTERVAL);
    void set_log_max_size(long size = LOG_DEFAULT_MAX_SIZE);

    // ----- getter 方法 -----
    const std::string& get_client_ipv4() const;
    unsigned int get_client_port() const;
    const std::string& get_cli_ipv4() const;
    unsigned int get_cli_port() const;
    long get_session_timeout() const;
    const std::string& get_db_path() const;
    const std::string& get_db_path_bak() const;
    const std::string& get_log_path() const;
    long get_log_cleanup_interval() const;
    long get_log_max_size() const;

    /**
     * @brief 从 YAML 配置文件读取配置
     * 
     * 若配置文件不存在，则创建默认配置文件并使用默认配置。
     * 配置文件路径为：可执行文件所在目录的父目录下的 service_config.yml。
     * 
     * @param msg 返回的消息
     * @return true  读取成功（或创建默认配置成功） false 读取失败（如文件格式错误）
     */
    bool read_config_from_file(std::string& msg);

private:
    // 私有构造函数，初始化默认配置
    AppConfig();

    /**
     * @brief 辅助函数：路径规范化，将路径中的反斜杠替换为正斜杠
     * 
     * @param path 路径
     * @return 规范化后的路径
     */
    std::string normalize_path_for_yaml(const std::string& path) const;

    // 写入默认配置文件（当文件不存在时调用）
    void write_default_config(const std::filesystem::path& path) const;

    // 成员变量（需互斥保护）
    mutable std::mutex config_mutex_;

    // 监听客户端配置
    std::string client_ipv4_;
    unsigned int client_port_;
    // 监听命令行工具配置
    std::string cli_ipv4_;
    unsigned int cli_port_;
    // 会话超时（秒）
    long session_timeout_;
    // 数据库配置
    std::string db_path_;
    std::string db_path_bak_;
    // 运行时日志配置
    std::string log_path_;
    long log_cleanup_interval_;
    long log_max_size_;
};

} // namespace zinpass::config

#endif // APP_CONFIG_H
