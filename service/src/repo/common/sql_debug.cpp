#include "repo/common/sql_debug.h"
#include "utils/log_manager.h"
#include <string>

namespace zinpass::repo {
    void SQLDebug::log_sql(sqlite3_stmt* stmt, bool release){
        if(release) return;

        if (char* expanded_sql = sqlite3_expanded_sql(stmt)) {
            utils::LogManager::AddLog(std::string("[SQL-DEBUG]: ") + expanded_sql);
            sqlite3_free(expanded_sql); // 必须手动释放内存
        }
    }
}