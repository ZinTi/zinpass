#include "config/app_config.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "utils/executable_path.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

namespace zinpass::config {

AppConfig& AppConfig::instance() {
    static AppConfig instance;
    return instance;
}

// ----- setter 实现 -----
void AppConfig::set_client_ipv4(const std::string& ipv4) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    client_ipv4_ = ipv4;
}

void AppConfig::set_client_port(unsigned int port) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    client_port_ = port;
}

void AppConfig::set_cli_ipv4(const std::string& ipv4) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    cli_ipv4_ = ipv4;
}

void AppConfig::set_cli_port(unsigned int port) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    cli_port_ = port;
}

void AppConfig::set_session_timeout(long timeout) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    session_timeout_ = timeout;
}

void AppConfig::set_db_path(const std::string& path) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    db_path_ = path;
}

void AppConfig::set_db_path_bak(const std::string& path) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    db_path_bak_ = path;
}

void AppConfig::set_log_path(const std::string& path) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    log_path_ = path;
}

void AppConfig::set_log_cleanup_interval(long interval) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    log_cleanup_interval_ = interval;
}

void AppConfig::set_log_max_size(long size) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    log_max_size_ = size;
}

// ----- getter 实现 -----
const std::string& AppConfig::get_client_ipv4() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return client_ipv4_;
}

unsigned int AppConfig::get_client_port() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return client_port_;
}

const std::string& AppConfig::get_cli_ipv4() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return cli_ipv4_;
}

unsigned int AppConfig::get_cli_port() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return cli_port_;
}

long AppConfig::get_session_timeout() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return session_timeout_;
}

const std::string& AppConfig::get_db_path() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return db_path_;
}

const std::string& AppConfig::get_db_path_bak() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return db_path_bak_;
}

const std::string& AppConfig::get_log_path() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return log_path_;
}

long AppConfig::get_log_cleanup_interval() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return log_cleanup_interval_;
}

long AppConfig::get_log_max_size() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return log_max_size_;
}

// 私有构造函数：初始化默认配置
AppConfig::AppConfig() {
    client_ipv4_ = RPC_DEFAULT_CLIENT_IPV4;
    client_port_ = RPC_DEFAULT_CLIENT_PORT;
    cli_ipv4_ = RPC_DEFAULT_CTRL_IPV4;
    cli_port_ = RPC_DEFAULT_CTRL_PORT;
    session_timeout_ = DEFAULT_SESSION_TIMEOUT;

    // 获取可执行文件的祖父目录（安装根目录）
    try {
        std::filesystem::path grandparent = zinpass::utils::get_executable_grandparent_dir();
        db_path_ = (grandparent / "data/zinpass.db").string();
        db_path_bak_ = (grandparent / "data/zinpass.bak.db").string();
        log_path_ = (grandparent / "data/zinpassd.log").string();
    } catch (const std::exception& e) {
        // std::cerr << "AppConfig Error: " << e.what() << std::endl;
        exit(1);
    }

    log_cleanup_interval_ = LOG_DEFAULT_CLEANUP_INTERVAL;
    log_max_size_ = 10 * LOG_DEFAULT_MAX_SIZE;
}

std::string AppConfig::normalize_path_for_yaml(const std::string& path) const {
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
void AppConfig::write_default_config(const std::filesystem::path& path) const {
    YAML::Node config;

    // 构建配置节点
    config["client"]["ipv4"] = client_ipv4_;
    config["client"]["port"] = client_port_;

    config["cli"]["ipv4"] = cli_ipv4_;
    config["cli"]["port"] = cli_port_;

    config["session"]["timeout"] = session_timeout_;

    // 对路径进行规范化处理
    config["database"]["path"] = normalize_path_for_yaml(db_path_);
    config["database"]["backup"] = normalize_path_for_yaml(db_path_bak_);

    config["log"]["path"] = normalize_path_for_yaml(log_path_);
    config["log"]["cleanup_interval"] = log_cleanup_interval_;
    config["log"]["max_size"] = log_max_size_;

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

bool AppConfig::read_config_from_file(std::string& msg) {
    // 构建配置文件路径：可执行文件所在目录的父目录 / service_config.yml
    std::filesystem::path conf_path;
    try {
        std::filesystem::path executable_path = zinpass::utils::get_executable_path();
        conf_path = executable_path.parent_path() / "service_config.yml";
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
        std::string new_client_ipv4 = client_ipv4_;
        unsigned int new_client_port = client_port_;
        std::string new_cli_ipv4 = cli_ipv4_;
        unsigned int new_cli_port = cli_port_;
        long new_session_timeout = session_timeout_;
        std::string new_db_path = db_path_;
        std::string new_db_path_bak = db_path_bak_;
        std::string new_log_path = log_path_;
        long new_log_cleanup_interval = log_cleanup_interval_;
        long new_log_max_size = log_max_size_;

        // 读取 client 部分
        if (config["client"] && config["client"]["ipv4"]) {
            new_client_ipv4 = config["client"]["ipv4"].as<std::string>();
        }
        if (config["client"] && config["client"]["port"]) {
            new_client_port = config["client"]["port"].as<unsigned int>();
        }

        // 读取 cli 部分
        if (config["cli"] && config["cli"]["ipv4"]) {
            new_cli_ipv4 = config["cli"]["ipv4"].as<std::string>();
        }
        if (config["cli"] && config["cli"]["port"]) {
            new_cli_port = config["cli"]["port"].as<unsigned int>();
        }

        // 读取 session 部分
        if (config["session"] && config["session"]["timeout"]) {
            new_session_timeout = config["session"]["timeout"].as<long>();
        }

        // 读取 database 部分
        if (config["database"] && config["database"]["path"]) {
            new_db_path = config["database"]["path"].as<std::string>();
#ifdef _WIN32
            // 在Windows上，将正斜杠转换回反斜杠以便系统使用
            std::replace(new_db_path.begin(), new_db_path.end(), '/', '\\');
#endif
        }
        if (config["database"] && config["database"]["backup"]) {
            new_db_path_bak = config["database"]["backup"].as<std::string>();
#ifdef _WIN32
            std::replace(new_db_path_bak.begin(), new_db_path_bak.end(), '/', '\\');
#endif
        }

        // 读取 log 部分
        if (config["log"] && config["log"]["path"]) {
            new_log_path = config["log"]["path"].as<std::string>();
#ifdef _WIN32
            std::replace(new_log_path.begin(), new_log_path.end(), '/', '\\');
#endif
        }
        if (config["log"] && config["log"]["cleanup_interval"]) {
            new_log_cleanup_interval = config["log"]["cleanup_interval"].as<long>();
        }
        if (config["log"] && config["log"]["max_size"]) {
            new_log_max_size = config["log"]["max_size"].as<long>();
        }

        // 一次性更新成员变量（加锁）
        {
            std::lock_guard<std::mutex> lock(config_mutex_);
            client_ipv4_ = std::move(new_client_ipv4);
            client_port_ = new_client_port;
            cli_ipv4_ = std::move(new_cli_ipv4);
            cli_port_ = new_cli_port;
            session_timeout_ = new_session_timeout;
            db_path_ = std::move(new_db_path);
            db_path_bak_ = std::move(new_db_path_bak);
            log_path_ = std::move(new_log_path);
            log_cleanup_interval_ = new_log_cleanup_interval;
            log_max_size_ = new_log_max_size;
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

} // namespace zinpass::config
