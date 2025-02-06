#include "state/state_manager.h"

namespace zinpass::state{

StateManager& StateManager::instance() {
    static StateManager instance;
    return instance;
}

//----- 用户状态组方法 -----
StateManager::UserState StateManager::get_user_state() const {
    std::lock_guard<std::mutex> lock(user_mutex_);
    return user_state_;
}

void StateManager::set_user_state(const UserState& state) {
    std::lock_guard<std::mutex> lock(user_mutex_);
    user_state_ = state;
}

//----- 应用配置组方法 -----
std::string StateManager::get_client_root_path() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return client_root_path_;
}

void StateManager::set_client_root_path(const std::string& path){
    std::lock_guard<std::mutex> lock(config_mutex_);
    client_root_path_ = path;
}

std::string StateManager::get_theme() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return client_theme_;
}

void StateManager::set_theme(const std::string& theme) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    client_theme_ = theme;
}

int StateManager::get_log_level() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return log_level_;
}

void StateManager::set_log_level(const int level) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    log_level_ = level;
}

//----- 独立计数器方法 -----
int StateManager::get_req_count() const {
    std::lock_guard<std::mutex> lock(counter_mutex_);
    return request_count_;
}

void StateManager::increment_req_count() {
    std::lock_guard<std::mutex> lock(counter_mutex_);
    ++request_count_;
}

}