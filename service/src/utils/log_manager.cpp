#include "utils/log_manager.h"
#include <fstream>
#include <iomanip>
#include <sstream>

#include "config/app_config.h"
#include "utils/date_time.h"

#if defined(_WIN32)
    #define LOG_DEFAULT_PATH "C:/Windows/Temp/zinpassd.log"
#else
    #define LOG_DEFAULT_PATH "/tmp/zinpassd.log"
#endif

namespace zinpass::utils {

// 获取单例实例的静态方法
LogManager& LogManager::instance() {
    static LogManager instance;
    return instance;
}

// 静态接口方法
void LogManager::add_log(const std::string& message) {
    instance().add_log_impl(message);
}

void LogManager::stop() {
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
    log_thread_ = std::thread(&LogManager::log_worker, this);
}

LogManager::~LogManager() {
    stop();
}

void LogManager::add_log_impl(const std::string& message) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        log_queue_.push(message);
    }
    cv_.notify_one();
}

void LogManager::log_worker() {
    std::string run_log_path = config::AppConfig::instance().get_log_path();   // 获取运行日志路径
    std::ofstream log_file;

    log_file.open(run_log_path, std::ios::app);

    if (!log_file.is_open()) {
        // 打开默认日志文件
        run_log_path = LOG_DEFAULT_PATH;
        config::AppConfig::instance().set_log_path(run_log_path);
        log_file.open(run_log_path, std::ios::app);

        if(!log_file.is_open()){
            // std::cerr << << utils::DateTime::getCurrentDateTime() << " [0x" << std::this_thread::get_id() << "] " << "Failed to open all log files. Subsequent logs will be lost." << std::endl;
            return;
        }
        add_log("Use default log file: " + run_log_path);
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
