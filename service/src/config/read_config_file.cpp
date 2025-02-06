#include "config/read_config_file.h"
#include <fstream>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

namespace zinpass::config {

    ReadConfigFile::ReadConfigFile() {
        this->database_path_ = "";
        this->run_log_path_ = "";
    }
    ReadConfigFile::~ReadConfigFile() {

    }

    void ReadConfigFile::read_config_from_file() {
        // 获取可执行文件路径
        std::string exe_dir;
        char buffer[1024] = {0};

#ifdef _WIN32
        HMODULE hModule = GetModuleHandle(nullptr);
        GetModuleFileNameA(hModule, buffer, sizeof(buffer));
        std::string exe_path(buffer);
        size_t pos = exe_path.find_last_of("\\/");
#else
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            std::string exe_path(buffer);
            size_t pos = exe_path.find_last_of('/');
#endif

        if (pos != std::string::npos) {
            exe_dir = exe_path.substr(0, pos);
        }
        else {
            exe_dir = ".";
        }

#ifndef _WIN32
    }
#endif

    // 构建配置文件路径
    std::string config_path = exe_dir + "/service.config";

    // 创建文件（如果不存在）
    std::ifstream test_file(config_path);
    if (!test_file) {
        std::ofstream create_file(config_path);
        if (create_file)
            create_file.close();
    }

    // 读取配置文件
    bool found_database = false;
    bool found_log = false;
    std::ifstream config_file(config_path);
    std::string line;

    while (std::getline(config_file, line)) {
        // 去除前后空格
        auto trim = [](std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
        };

        trim(line);
        if (line.empty())
            continue;

        // 解析键值对
        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) continue;

        std::string key = line.substr(0, eq_pos);
        trim(key);
        std::string value = line.substr(eq_pos + 1);
        trim(value);

        // 解析带引号的值
        std::string clean_value;
        if (!value.empty()) {
            if (value.front() == '"' || value.front() == '\'') {
                size_t end_quote = value.find(value.front(), 1);
                if (end_quote != std::string::npos) {
                    clean_value = value.substr(1, end_quote - 1);
                } else { // 引号不匹配时使用整个值
                    clean_value = value.substr(1); // 去掉开头的引号
                }
            } else {
                clean_value = value;
            }
        } else {
            clean_value = value; // 空值处理
        }

        // 存储配置值
        if (key == "database") {    // 数据库配置
            database_path_ = clean_value;
            found_database = true;
        } else if (key == "log") {  // 日志配置
            run_log_path_ = clean_value;
            found_log = true;
        }
    }

    // 设置默认路径
    if (!found_database) database_path_ = exe_dir + "/zinpass.db";  // 默认数据库路径
    if (!found_log) run_log_path_ = exe_dir + "/service.log";           // 默认运行日志路径

    // 统一路径分隔符（日志路径处理）
#ifdef _WIN32
    std::replace(database_path_.begin(), database_path_.end(), '/', '\\');
    std::replace(run_log_path_.begin(), run_log_path_.end(), '/', '\\');
#else
    std::replace(database_path_.begin(), database_path_.end(), '\\', '/');
    std::replace(run_log_path_.begin(), run_log_path_.end(), '\\', '/');
#endif
}

std::string ReadConfigFile::get_db_path(){
    return database_path_;
}

std::string ReadConfigFile::get_run_log_path() {
    return run_log_path_;
}

}

