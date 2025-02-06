#include "repo/telecom_operator_dao.h"
#include "repo/common/sql_debug.h"
#include "utils/log_manager.h"
extern "C" {
    #include <sqlite3.h>
}

namespace zinpass::repo{

TelecomOperatorDAO::TelecomOperatorDAO() : pool_(ConnectionPool::get_instance()){
}
TelecomOperatorDAO::~TelecomOperatorDAO() = default;

DaoStatus TelecomOperatorDAO::findList(std::vector<std::string>& telecom_operators) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT name FROM telecom_operator ORDER BY id ASC";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        telecom_operators.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    utils::LogManager::AddLog(std::string("[OK] Number of Telecom Operator records found: ") + std::to_string(telecom_operators.size()));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

}
