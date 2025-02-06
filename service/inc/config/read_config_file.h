#ifndef READ_CONFIG_FILE_H
#define READ_CONFIG_FILE_H

#include <string>

namespace zinpass::config{

class ReadConfigFile{
    public:
        ReadConfigFile();
        ~ReadConfigFile();

        void read_config_from_file(); // 从配置文件中读取数据库配置
        std::string get_db_path();
        std::string get_run_log_path();

    private:
        std::string database_path_;    // 数据库路径
        std::string run_log_path_;     // 运行日志路径
};

}

#endif
