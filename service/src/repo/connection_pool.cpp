#include "repo/connection_pool.h"
#include <stdexcept>
#include "utils/log_manager.h"

namespace zinpass::repo {

std::unique_ptr<ConnectionPool> ConnectionPool::instance_;
std::once_flag ConnectionPool::init_flag_;

void ConnectionPool::initialize(const std::string& db_path, size_t pool_size) {
    std::call_once(init_flag_, [&]() {
        instance_.reset(new ConnectionPool(db_path, pool_size));
    });
}

ConnectionPool& ConnectionPool::get_instance() {
    if (!instance_) {
        utils::LogManager::AddLog("[ERROR] ConnectionPool not initialized");
        throw std::runtime_error("ConnectionPool not initialized");
    }
    return *instance_;
}

ConnectionPool::ConnectionPool(const std::string& db_path, const size_t pool_size)
    : db_path_(db_path), pool_size_(pool_size) {
    for (size_t i = 0; i < pool_size_; ++i) {
        sqlite3* conn = nullptr;
        // utils::LogManager::AddLog("[OK] Opening database connection");
        const int rc = sqlite3_open(db_path_.c_str(), &conn);
        if (rc != SQLITE_OK) {
            sqlite3_close(conn);
            const std::string err_msg(sqlite3_errmsg(conn));
            utils::LogManager::AddLog(std::string("[ERROR] Failed to open database: ") + err_msg);
            throw std::runtime_error("Failed to open database: " + err_msg);
        }

        // 启用外键约束
        char* errMsg = nullptr;
        const int pragma_rc = sqlite3_exec(conn, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errMsg);
        if (pragma_rc != SQLITE_OK) {
            const std::string pragma_err = errMsg ? errMsg : "Unknown error";
            sqlite3_free(errMsg);
            sqlite3_close(conn);
            utils::LogManager::AddLog("[ERROR] Failed to enable foreign keys: " + pragma_err);
            throw std::runtime_error("Failed to enable foreign keys: " + pragma_err);
        }

        available_connections_.push(conn);
    }
}

ConnectionPool::~ConnectionPool() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!available_connections_.empty()) {
        sqlite3* conn = available_connections_.front();
        // utils::LogManager::AddLog("[OK] Closing database connection");
        sqlite3_close(conn);
        available_connections_.pop();
    }
}

sqlite3* ConnectionPool::get_connection() {
    // utils::LogManager::AddLog("[OK] get_connection()");
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this] { return !available_connections_.empty(); });

    sqlite3* conn = available_connections_.front();
    available_connections_.pop();
    return conn;
}

void ConnectionPool::release_connection(sqlite3* conn) {
    // utils::LogManager::AddLog("[OK] release_connection(conn)");
    std::unique_lock<std::mutex> lock(mutex_);
    available_connections_.push(conn);
    lock.unlock();
    condition_.notify_one();
}
}