#ifndef INIT_DATABASE_H
#define INIT_DATABASE_H

#include <string>
extern "C" {
    #include <sqlite3.h>
}

namespace zinpass::repo {
class InitDatabase{
public:
    /**
     * @brief InitDatabase 初始化DB构造函数
     * @param db_path 数据库文件路径
     */
    explicit InitDatabase(const std::string& db_path);

    /**
     * @brief init 执行初始化操作
     * @return 返回 0 代表成功
     */
    [[nodiscard]] int init() const;
private:
    std::string db_path_;
    static int executeSQL(sqlite3* conn, const std::string& sql);
};
}

#endif
