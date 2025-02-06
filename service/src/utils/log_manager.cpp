#include "utils/log_manager.h"
#include <fstream>
#include <iomanip>
#include <sstream>

#include "config/state_manager.h"
#include "utils/date_time.h"

namespace zinpass::utils {

// 获取单例实例的静态方法
LogManager& LogManager::GetInstance() {
    static LogManager instance;
    return instance;
}

// 静态接口方法
void LogManager::AddLog(const std::string& message) {
    GetInstance().AddLogImpl(message);
}

void LogManager::Stop() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        stop_requested_ = true;
    }
    cv_.notify_one();

    if (log_thread_.joinable()) {
        log_thread_.join();
    }
}

LogManager::LogManager() {
    log_thread_ = std::thread(&LogManager::LogWorker, this);
}

LogManager::~LogManager() {
    Stop();
}

void LogManager::AddLogImpl(const std::string& message) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        log_queue_.push(message);
    }
    cv_.notify_one();
}

void LogManager::LogWorker() {
    const std::string run_log_path = config::StateManager::instance().get_run_log_path();   // 获取运行日志路径
    std::ofstream log_file(run_log_path, std::ios::app);
    if (!log_file.is_open()) {
        // std::cerr << "Failed to open log file!" << std::endl;
        return;
    }

    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        cv_.wait(lock, [this]() {
            return !log_queue_.empty() || stop_requested_;
        });

        while (!log_queue_.empty()) {
            auto msg = log_queue_.front();
            log_queue_.pop();
            lock.unlock();

            std::stringstream ss;
            ss << utils::DateTime::getCurrentDateTime()
               << " [0x" << std::this_thread::get_id() << "] " << msg;

            log_file << ss.str() << std::endl;
            log_file.flush();

            lock.lock();
        }

        if (stop_requested_) break;
    }

    log_file.close();
}

}