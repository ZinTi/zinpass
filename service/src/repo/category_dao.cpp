#include "repo/category_dao.h"
#include "repo/common/sql_debug.h"
#include "utils/log_manager.h"
extern "C" {
#include <sqlite3.h>
}

namespace zinpass::repo{

CategoryDAO::CategoryDAO() : pool_(ConnectionPool::get_instance()) {
}

CategoryDAO::~CategoryDAO() = default;

DaoStatus CategoryDAO::categoryNameToId(const std::string& categoryName, short* id) const {
    DaoStatus status;
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id FROM account_category WHERE name = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, categoryName.c_str(), static_cast<int>(categoryName.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW == sqlite3_step(stmt)) {
        *id = static_cast<short>(sqlite3_column_int(stmt, 0));
        utils::LogManager::AddLog("[OK] Category ID found.");
        status = DaoStatus::Success;
    }else {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Category ID not found with the given category name in the database."));
        status = DaoStatus::GenericError;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

DaoStatus CategoryDAO::findCategories(std::vector<std::string>& categories) const {
    sqlite3* conn = pool_.get_connection();
    const std::string sql = "SELECT name FROM account_category";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        categories.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    utils::LogManager::AddLog(std::string("[OK] Number of category records found: ") + std::to_string(categories.size()));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

}