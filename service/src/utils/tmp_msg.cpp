#include "utils/tmp_msg.h"
#include <deque>
#include <mutex>
#include <string>

static std::deque<std::string> log_msg;    // 日志消息队列
static std::mutex log_msg_mutex;           // 保护log_msg的互斥锁

bool tmp_msg_empty() {
    std::lock_guard<std::mutex> lock(log_msg_mutex);
    return log_msg.empty();
}

void tmp_msg_add(const std::string& msg) {
    std::lock_guard<std::mutex> lock(log_msg_mutex);
    log_msg.push_back(msg);
}

std::string tmp_msg_get() {
    std::lock_guard<std::mutex> lock(log_msg_mutex);
    
    if (log_msg.empty()) {
        return "";
    }
    
    std::string msg = log_msg.front();
    log_msg.pop_front();
    return msg;
}

std::string tmp_msg_take_all(const std::string& delimiter) {
    std::lock_guard<std::mutex> lock(log_msg_mutex);
    
    std::string all_msg;
    bool first = true;
    for (auto& msg : log_msg) {
        if (!first) {
            all_msg += delimiter;
        }
        all_msg += msg;
        first = false;
    }
    
    log_msg.clear();
    return all_msg;
}
