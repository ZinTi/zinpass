#include "state/client_config.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <QMessageBox>
#include "utils/executable_path.h"

namespace zinpass::state {

ClientConfig& ClientConfig::instance() {
    static ClientConfig instance;
    return instance;
}

// ----- setter -----
void ClientConfig::set_server_ipv4(const std::string& ipv4){
    std::lock_guard<std::mutex> lock(config_mutex_);
    server_ipv4_ = ipv4;
}
void ClientConfig::set_server_port(unsigned int port){
    std::lock_guard<std::mutex> lock(config_mutex_);
    server_port_ = port;
}
void ClientConfig::set_preferences_theme(const std::string& theme){
    std::lock_guard<std::mutex> lock(config_mutex_);
    preferences_theme_ = theme;
}
void ClientConfig::set_preferences_locale(const std::string& locale){
    std::lock_guard<std::mutex> lock(config_mutex_);
    preferences_locale_ = locale;
}
void ClientConfig::set_log_path(const std::string& path){
    std::lock_guard<std::mutex> lock(config_mutex_);
    log_path_ = path;
}

// ----- getter -----
const std::string& ClientConfig::get_server_ipv4() const{
    std::lock_guard<std::mutex> lock(config_mutex_);
    return server_ipv4_;
}
unsigned int ClientConfig::get_server_port() const{
    std::lock_guard<std::mutex> lock(config_mutex_);
    return server_port_;
}
const std::string& ClientConfig::get_preferences_theme() const{
    std::lock_guard<std::mutex> lock(config_mutex_);
    return preferences_theme_;
}
const std::string& ClientConfig::get_preferences_locale() const{
    std::lock_guard<std::mutex> lock(config_mutex_);
    return preferences_locale_;
}
const std::string& ClientConfig::get_log_path() const{
    std::lock_guard<std::mutex> lock(config_mutex_);
    return log_path_;
}

ClientConfig::ClientConfig() {
    server_ipv4_ = RPC_DEFAULT_SERVER_IPV4;
    server_port_ = RPC_DEFAULT_SERVER_PORT;
    preferences_theme_ = PREFERENCES_DEFAULT_THEME;
    preferences_locale_ = PREFERENCES_DEFAULT_LOCALE;

    // 设置默认日志路径
    try {
        std::filesystem::path grandparent = zinpass::utils::get_executable_grandparent_dir();
        log_path_ = (grandparent / "data/client.log").string();
    } catch (const std::exception& e) {
        // QMessageBox::critical(nullptr, "错误", QString("配置文件读取失败: %1").arg(QString::fromStdString(e.what()));
        exit(1);
    }
}

std::string ClientConfig::normalize_path_for_yaml(const std::string& path) const {
    std::string result = path;
#ifdef _WIN32
    // 将反斜杠转换为正斜杠
    std::replace(result.begin(), result.end(), '\\', '/');
#endif
    return result;
}

/**
 * @brief 写入默认配置文件（YAML格式）
 * @param path 配置文件路径
 */
void ClientConfig::write_default_config(const std::filesystem::path& path) const {
    YAML::Node config;

    // 构建配置节点
    // 服务器地址和端口
    config["server"]["ipv4"] = server_ipv4_;
    config["server"]["port"] = server_port_;
    // 用户偏好设置
    config["preferences"]["theme"] = preferences_theme_;
    config["preferences"]["locale"] = preferences_locale_;
    // 客户端运行日志
    config["log"]["path"] = normalize_path_for_yaml(log_path_);

    // 创建父目录（如果不存在）
    std::filesystem::create_directories(path.parent_path());

    // 写入文件，使用 YAML::Emitter 以获得更好的控制
    std::ofstream fout(path.string());
    if (!fout.is_open()) {
        throw std::runtime_error("Failed to create configuration: " + path.string());
    }
    
    YAML::Emitter emitter;
    emitter << config;
    fout << emitter.c_str();
}

bool ClientConfig::read_config_from_file(std::string& msg) {
    // 构建配置文件路径：可执行文件所在目录的父目录 / client_config.yml
    std::filesystem::path conf_path;
    try {
        std::filesystem::path executable_path = zinpass::utils::get_executable_path();
        conf_path = executable_path.parent_path() / "client_config.yml";
    } catch (const std::exception& e) {
        msg = "Failed to obtain the path of the executable file: " + std::string(e.what());
        return false;
    }

    // 若文件不存在，创建默认配置文件
    if (!std::filesystem::exists(conf_path)) {
        try {
            write_default_config(conf_path);
            msg = "The configuration does not exist. A default configuration has been created:  " + conf_path.string();
            // 当前成员变量已经是默认值，无需更新
            return true;
        } catch (const std::exception& e) {
            msg = "Failed to create the default configuration: " + std::string(e.what());
            return false;
        }
    }

    // 读取配置文件
    try {
        YAML::Node config = YAML::LoadFile(conf_path.string());

        // 使用局部变量暂存，最后统一加锁更新，减少锁持有时间
        std::string new_server_ipv4 = server_ipv4_;
        unsigned int new_server_port = server_port_;
        std::string new_preferences_theme = preferences_theme_;
        std::string new_preferences_locale = preferences_locale_;
        std::string new_log_path = log_path_;

        // 读取 server 部分
        if (config["server"] && config["server"]["ipv4"]) {
            new_server_ipv4 = config["server"]["ipv4"].as<std::string>();
        }
        if (config["server"] && config["server"]["port"]) {
            new_server_port = config["server"]["port"].as<unsigned int>();
        }

        // 读取 preferences 部分
        if (config["preferences"] && config["preferences"]["theme"]) {
            new_preferences_theme = config["preferences"]["theme"].as<std::string>();
        }
        if (config["preferences"] && config["preferences"]["locale"]) {
            new_preferences_locale = config["preferences"]["locale"].as<std::string>();
        }

        // 读取 log 部分
        if (config["log"] && config["log"]["path"]) {
            new_log_path = config["log"]["path"].as<std::string>();
#ifdef _WIN32
            std::replace(new_log_path.begin(), new_log_path.end(), '/', '\\');
#endif
        }

        // 一次性更新成员变量（加锁）
        {
            std::lock_guard<std::mutex> lock(config_mutex_);
            server_ipv4_ = std::move(new_server_ipv4);
            server_port_ = new_server_port;

            preferences_theme_ = std::move(new_preferences_theme);
            preferences_locale_ = std::move(new_preferences_locale);

            log_path_ = std::move(new_log_path);

        }

        return true;

    } catch (const YAML::Exception& e) {
        msg = "Failed to parse the configuration: " + std::string(e.what());
        return false;
    } catch (const std::exception& e) {
        msg = "An exception occurred while reading the configuration: " + std::string(e.what());
        return false;
    }
}

} // namespace zinpass::state
