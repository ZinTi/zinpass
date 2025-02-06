#include "repo/user_dao.h"
extern "C" {
#include <sqlite3.h>
}
#include "utils/log_manager.h"
#include "repo/common/sql_debug.h"

namespace zinpass::repo{

SystemUserDAO::SystemUserDAO() : pool_(ConnectionPool::get_instance()) {
}

SystemUserDAO::~SystemUserDAO() = default;

DaoStatus SystemUserDAO::findById(const std::string& id, models::SystemUser& sys_user) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id, username, nickname, hashed_pwd, salt, created_time, updated_time FROM system_user WHERE id = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] User not found with the given ID in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::NotFound;
    }

    sys_user.setId(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
    sys_user.setUsername(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1)));
    sys_user.setNickname(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 2)));
    sys_user.setPassword(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 3)));
    sys_user.setSalt(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 4)));
    sys_user.setCreatedTime(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 5)));
    sys_user.setUpdatedTime(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 6)));
    utils::LogManager::AddLog("[OK] User found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus SystemUserDAO::findByUsername(const std::string& username, models::SystemUser& sys_user) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id, username, nickname, hashed_pwd, salt, created_time, updated_time FROM system_user WHERE username = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), static_cast<int>(username.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] User not found with the given username in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::NotFound;
    }

    sys_user.setId(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
    sys_user.setUsername(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1)));
    sys_user.setNickname(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 2)));
    sys_user.setPassword(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 3)));
    sys_user.setSalt(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 4)));
    sys_user.setCreatedTime(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 5)));
    sys_user.setUpdatedTime(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 6)));
    utils::LogManager::AddLog("[OK] User found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus SystemUserDAO::findUsernames(std::vector<std::string>& usernames) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT username FROM system_user;";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        const std::string username = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0));
        usernames.push_back(username);
    }

    utils::LogManager::AddLog(std::string("[OK] Number of Username records found: ") + std::to_string(usernames.size()));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus SystemUserDAO::add(const models::SystemUser& sys_user) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "INSERT INTO system_user(id, username, nickname, hashed_pwd, salt, created_time, updated_time) VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    sqlite3_bind_text(stmt, 1, sys_user.getId().c_str(), static_cast<int>(sys_user.getId().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, sys_user.getUsername().c_str(), static_cast<int>(sys_user.getUsername().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, sys_user.getNickname().c_str(), static_cast<int>(sys_user.getNickname().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, sys_user.getPassword().c_str(), static_cast<int>(sys_user.getPassword().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, sys_user.getSalt().c_str(), static_cast<int>(sys_user.getSalt().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, sys_user.getCreatedTime().c_str(), static_cast<int>(sys_user.getCreatedTime().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, sys_user.getUpdatedTime().c_str(), static_cast<int>(sys_user.getUpdatedTime().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    DaoStatus status;
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to insert system_user record: ") + std::string(sqlite3_errmsg(conn)));
        status = DaoStatus::DuplicateKey;
    }else {
        utils::LogManager::AddLog(std::string("[OK] New user created successfully: ") + sys_user.getId());
        status = DaoStatus::Success;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

DaoStatus SystemUserDAO::update(const models::SystemUser& sys_user) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE system_user SET username = ?, nickname = ?, hashed_pwd = ?, salt = ?, updated_time = ? WHERE id = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    sqlite3_bind_text(stmt, 1, sys_user.getUsername().c_str(), static_cast<int>(sys_user.getUsername().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, sys_user.getNickname().c_str(), static_cast<int>(sys_user.getNickname().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, sys_user.getPassword().c_str(), static_cast<int>(sys_user.getPassword().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, sys_user.getSalt().c_str(), static_cast<int>(sys_user.getSalt().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, sys_user.getUpdatedTime().c_str(), static_cast<int>(sys_user.getUpdatedTime().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, sys_user.getId().c_str(), static_cast<int>(sys_user.getId().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }
    utils::LogManager::AddLog(std::string("[OK] User updated successfully: ") + sys_user.getId());
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus SystemUserDAO::remove(const std::string& id) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "DELETE FROM system_user WHERE id = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);

    DaoStatus status;
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to remove record: ") + std::string(sqlite3_errmsg(conn)));
        status = DaoStatus::GenericError;
    }else {
        utils::LogManager::AddLog(std::string("[OK] User removed successfully: ") + id);
        status = DaoStatus::Success;
    }

    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

}