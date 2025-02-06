#include "mod_session/session_dao.h"
#include "repo/common/sql_debug.h"
#include "utils/log_manager.h"

namespace zinpass::mod_session {
    SessionDAO::SessionDAO() : pool_(repo::ConnectionPool::get_instance()) {
    }

    SessionDAO::~SessionDAO() = default;

    repo::DaoStatus SessionDAO::find_by_id(const std::string& id, mod_session::Session& session) const {
        sqlite3* conn = pool_.get_connection();
        const std::string& sql = "SELECT id, sys_user_id, data, created_time, expires_time, accessed_time FROM session WHERE id = ?";
        sqlite3_stmt* stmt;
        if (SQLITE_OK != sqlite3_prepare(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::InvalidData;
        }
        sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
        repo::SQLDebug::log_sql(stmt, true);
        if (SQLITE_ROW != sqlite3_step(stmt)) {
            utils::LogManager::AddLog(std::string("[NOT FOUND] Session not found with the given ID in the database."));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::NotFound;
        }

        const auto* session_id = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0));
        session.setId(session_id ? session_id : "");
        const auto* sys_user_id = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1));
        session.setSysUserId(sys_user_id ? sys_user_id : "");
        const auto* data = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 2));
        session.setData(data ? data : "");
        const auto* created_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 3));
        session.setCreatedTime(created_time ? created_time : "");
        const auto* expires_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 4));
        session.setExpiresTime(expires_time ? expires_time : "");
        const auto* accessed_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 5));
        session.setAccessedTime(accessed_time ? accessed_time : "");
        // utils::LogManager::AddLog("[OK] Session found.");
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return repo::DaoStatus::Success;
    }

    std::unordered_map<std::string, std::string> SessionDAO::find_id_and_expire() const {
        std::unordered_map<std::string, std::string> result;
        sqlite3* conn = pool_.get_connection();
        const std::string& sql = "SELECT id, expires_time FROM session";
        sqlite3_stmt* stmt;
        if (SQLITE_OK != sqlite3_prepare(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return result;
        }
        repo::SQLDebug::log_sql(stmt, true);
        while (SQLITE_ROW == sqlite3_step(stmt)) {
            const auto* session_id = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0));
            const auto* expires_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1));
            if (session_id) {
                result[session_id] = expires_time ? expires_time : "";
            }
        }
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return result;
    }

    repo::DaoStatus SessionDAO::add(const mod_session::Session& session) const {
        sqlite3* conn = pool_.get_connection();
        const std::string sql = "INSERT INTO session(id, sys_user_id, data, created_time, expires_time, accessed_time) VALUES (:id, :sys_user_id, :data, :created_time, :expires_time, :accessed_time)";
        sqlite3_stmt* stmt;
        if (SQLITE_OK != sqlite3_prepare(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::InvalidData;
        }
        sqlite3_bind_text(stmt, 1, session.getId().c_str(), static_cast<int>(session.getId().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, session.getSysUserId().c_str(), static_cast<int>(session.getSysUserId().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, session.getData().c_str(), static_cast<int>(session.getData().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, session.getCreatedTime().c_str(), static_cast<int>(session.getCreatedTime().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, session.getExpiresTime().c_str(), static_cast<int>(session.getExpiresTime().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, session.getAccessedTime().c_str(), static_cast<int>(session.getAccessedTime().size()), SQLITE_TRANSIENT);
        repo::SQLDebug::log_sql(stmt, true);
        repo::DaoStatus status;
        const int result = sqlite3_step(stmt);
        if (SQLITE_DONE == result) {
            status = repo::DaoStatus::Success;
            utils::LogManager::AddLog("[OK] New Session created: " + session.getId());
        } else {
            const char* errMsg = sqlite3_errmsg(conn); // 在释放资源前获取错误信息（避免连接释放后丢失）
            switch (result) {
            case SQLITE_CONSTRAINT: {
                status = repo::DaoStatus::DuplicateKey; // 约束冲突（唯一键、外键、非空约束等）
                utils::LogManager::AddLog(
                    "[ERROR] Constraint violation: " + std::string(errMsg) +
                    " | Session ID: " + session.getId()
                );
                break;
            }
            case SQLITE_BUSY: {
                status = repo::DaoStatus::DatabaseBusy; // 数据库被锁定（如其他连接持有写锁）
                utils::LogManager::AddLog(
                    "[ERROR] Database busy: " + std::string(errMsg) +
                    " | Check transactions or locks."
                );
                break;
            }
            case SQLITE_READONLY: {
                status = repo::DaoStatus::ReadOnlyError; // 数据库文件只读或权限不足
                utils::LogManager::AddLog(
                    "[ERROR] Write to read-only DB: " + std::string(errMsg)
                );
                break;
            }
            case SQLITE_MISUSE: {
                status = repo::DaoStatus::ConfigurationError; // API使用错误（如未正确关闭stmt）
                utils::LogManager::AddLog(
                    "[ERROR] SQLite API misuse: " + std::string(errMsg) +
                    " | Check connection/stmt lifecycle."
                );
                break;
            }
            case SQLITE_ERROR:
            default: {
                status = repo::DaoStatus::GenericError; // 通用错误（如数据类型转换失败、SQL语法错误等）
                utils::LogManager::AddLog(
                    "[ERROR] SQL operation failed. Code: " + std::to_string(result) +
                    " | Details: " + std::string(errMsg)
                );
                break;
            }
            } // switch
        } // if-else

        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return status;
    }

    repo::DaoStatus SessionDAO::update(const mod_session::Session& session) const {
        sqlite3* conn = pool_.get_connection();
        const std::string sql = "UPDATE session SET sys_user_id =?, data =?, created_time =?, expires_time =?, accessed_time =? WHERE id = ?";
        sqlite3_stmt* stmt;
        if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::InvalidData;
        }
        sqlite3_bind_text(stmt, 1, session.getSysUserId().c_str(), static_cast<int>(session.getSysUserId().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, session.getData().c_str(), static_cast<int>(session.getData().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, session.getCreatedTime().c_str(), static_cast<int>(session.getCreatedTime().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, session.getExpiresTime().c_str(), static_cast<int>(session.getExpiresTime().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, session.getAccessedTime().c_str(), static_cast<int>(session.getAccessedTime().size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, session.getId().c_str(), static_cast<int>(session.getId().size()), SQLITE_TRANSIENT);
        repo::SQLDebug::log_sql(stmt, true);
        if (SQLITE_DONE != sqlite3_step(stmt)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to update session: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::GenericError;
        }
        utils::LogManager::AddLog(std::string("[OK] Session updated successfully: ") + session.getId());
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return repo::DaoStatus::Success;
    }

    repo::DaoStatus SessionDAO::remove(const std::string& id) const {
        sqlite3* conn = pool_.get_connection();

        const std::string sql = "DELETE FROM session WHERE id = ?";
        sqlite3_stmt* stmt;
        if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::InvalidData;
        }
        sqlite3_bind_text(stmt, 1, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);
        repo::SQLDebug::log_sql(stmt, true);
        if (SQLITE_DONE != sqlite3_step(stmt)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to remove record: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::GenericError;
        }

        utils::LogManager::AddLog(std::string("[OK] Session removed successfully: ") + id);
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return repo::DaoStatus::Success;
    }

    int SessionDAO::remove_all(void) const {
        sqlite3* conn = pool_.get_connection();
        int rows_deleted = 0;

        const std::string sql = "DELETE FROM session";
        sqlite3_stmt* stmt = nullptr;

        // 准备语句
        if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(
                std::string("[ERROR] Failed to prepare statement: ") +
                std::string(sqlite3_errmsg(conn))
            );
            if (stmt) sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return 0;
        }

        // 调试日志
        repo::SQLDebug::log_sql(stmt, true);

        // 执行删除
        int step_result = sqlite3_step(stmt);
        if (step_result != SQLITE_DONE) {
            utils::LogManager::AddLog(
                std::string("[ERROR] Failed to remove records: ") +
                std::string(sqlite3_errmsg(conn))
            );
        } else {
            // 获取实际删除的行数
            rows_deleted = sqlite3_changes(conn);
            utils::LogManager::AddLog(
                std::string("[OK] Removed ") +
                std::to_string(rows_deleted) +
                std::string(" session records")
            );
        }

        // 清理资源
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);

        return rows_deleted;
    }

    repo::DaoStatus SessionDAO::update_expiration(const std::string& id, const std::string& expires_date_time) const{
        sqlite3* conn = pool_.get_connection();
        const std::string sql = "UPDATE session SET expires_time =? WHERE id = ?";
        sqlite3_stmt* stmt;
        if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::InvalidData;
        }

        sqlite3_bind_text(stmt, 1, expires_date_time.c_str(), static_cast<int>(expires_date_time.size()), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, id.c_str(), static_cast<int>(id.size()), SQLITE_TRANSIENT);

        repo::SQLDebug::log_sql(stmt, true);

        if (SQLITE_DONE != sqlite3_step(stmt)) {
            utils::LogManager::AddLog(std::string("[ERROR] Failed to update session expiration: ") + std::string(sqlite3_errmsg(conn)));
            sqlite3_finalize(stmt);
            pool_.release_connection(conn);
            return repo::DaoStatus::GenericError;
        }
        utils::LogManager::AddLog(std::string("[OK] Session expiration updated successfully: ") + id);
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return repo::DaoStatus::Success;
    }

}