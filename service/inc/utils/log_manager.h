#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace zinpass::utils {

class LogManager {
public:
    // 删除拷贝构造函数和赋值运算符
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    // 获取单例实例的静态方法
    static LogManager& GetInstance();

    // 静态接口方法
    static void AddLog(const std::string& message);

    void Stop();

private:
    LogManager();

    ~LogManager();

    void AddLogImpl(const std::string& message);

    void LogWorker();

    std::queue<std::string> log_queue_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    std::thread log_thread_;
    std::atomic<bool> stop_requested_{false};
};

}

#endif
