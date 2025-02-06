#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include <string>
#include <mutex>
#include <filesystem>

#define RPC_DEFAULT_SERVER_IPV4             "127.0.0.1"
#define RPC_DEFAULT_SERVER_PORT             50051
#define PREFERENCES_DEFAULT_THEME           "system"
#define PREFERENCES_DEFAULT_LOCALE          "zh_CN"

namespace zinpass::state {

class ClientConfig {
public:
    // 禁止拷贝构造函数和赋值运算符
    ClientConfig(const ClientConfig&) = delete;
    ClientConfig& operator=(const ClientConfig&) = delete;

    static ClientConfig& instance();

    // ----- setter -----
    void set_server_ipv4(const std::string& ipv4 = RPC_DEFAULT_SERVER_IPV4);
    void set_server_port(unsigned int port = RPC_DEFAULT_SERVER_PORT);
    void set_preferences_theme(const std::string& theme = PREFERENCES_DEFAULT_THEME);
    void set_preferences_locale(const std::string& locale = PREFERENCES_DEFAULT_LOCALE);
    void set_log_path(const std::string& path);

    // ----- getter -----
    const std::string& get_server_ipv4() const;
    unsigned int get_server_port() const;
    const std::string& get_preferences_theme() const;
    const std::string& get_preferences_locale() const;
    const std::string& get_log_path() const;

    /**
     * @brief 从 YAML 配置文件中读取配置信息
     * 
     * 若配置文件不存在，则创建默认配置文件并使用默认配置。
     * 配置文件路径为：可执行文件所在目录的父目录下的 client_config.yml。
     * 
     * @param msg 返回的信息
     * @return true 读取成功，false 读取失败
     */
    bool read_config_from_file(std::string& msg);

private:
    // 私有构造函数
    ClientConfig();

    /**
     * @brief 写入默认配置文件（当文件不存在时调用）
     */
    void write_default_config(const std::filesystem::path& conf_path) const;

    /**
     * @brief 辅助函数：路径规范化，将路径中的反斜杠替换为正斜杠
     * 
     * @param path 路径
     * @return 规范化后的路径
     */
    std::string normalize_path_for_yaml(const std::string& path) const;

    // 成员变量（需互斥保护）
    mutable std::mutex config_mutex_;

    std::string server_ipv4_;
    unsigned int server_port_;

    std::string preferences_theme_;
    std::string preferences_locale_;

    std::string log_path_;

};

} // namespace zinpass::state

#endif // CLIENT_CONFIG_H
