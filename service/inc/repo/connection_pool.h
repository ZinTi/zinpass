#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>
#include <atomic>
extern "C" {
#include <sqlite3.h>
}

namespace zinpass::repo {

    class ConnectionPool {
    public:
        static void initialize(const std::string& db_path, size_t pool_size);
        static ConnectionPool& get_instance();

        sqlite3* get_connection();
        void release_connection(sqlite3* conn);

        ConnectionPool(const ConnectionPool&) = delete;
        ConnectionPool& operator=(const ConnectionPool&) = delete;

    private:
        ConnectionPool(const std::string& db_path, size_t pool_size);
        ~ConnectionPool();

        // default_delete的友元声明
        friend class std::default_delete<ConnectionPool>;

        std::string db_path_;
        size_t pool_size_;
        std::queue<sqlite3*> available_connections_;
        std::mutex mutex_;
        std::condition_variable condition_;

        static std::unique_ptr<ConnectionPool> instance_;
        static std::once_flag init_flag_;
    };

}

#endif //CONNECTION_POOL_H
