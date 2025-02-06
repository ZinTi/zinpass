#include "state_manager.h"

namespace zinpass::state{

StateManager& StateManager::instance() {
    static StateManager instance;
    return instance;
}

//----- 用户状态组方法 -----
StateManager::UserState StateManager::getUserState() const {
    std::lock_guard<std::mutex> lock(userMutex_);
    return userState_;
}

void StateManager::setUserState(const UserState& state) {
    std::lock_guard<std::mutex> lock(userMutex_);
    userState_ = state;
}

//----- 应用配置组方法 -----
std::string StateManager::getClientRootPath() const {
    std::lock_guard<std::mutex> lock(configMutex_);
    return clientRootPath_;
}

void StateManager::setClientRootPath(const std::string& path){
    std::lock_guard<std::mutex> lock(configMutex_);
    clientRootPath_ = path;
}

std::string StateManager::getTheme() const {
    std::lock_guard<std::mutex> lock(configMutex_);
    return currentTheme_;
}

void StateManager::setTheme(const std::string& theme) {
    std::lock_guard<std::mutex> lock(configMutex_);
    currentTheme_ = theme;
}

int StateManager::getLogLevel() const {
    std::lock_guard<std::mutex> lock(configMutex_);
    return logLevel_;
}

void StateManager::setLogLevel(const int level) {
    std::lock_guard<std::mutex> lock(configMutex_);
    logLevel_ = level;
}

//----- 独立计数器方法 -----
int StateManager::getReqCount() const {
    std::lock_guard<std::mutex> lock(counterMutex_);
    return requestCount_;
}

void StateManager::incrementReqCount() {
    std::lock_guard<std::mutex> lock(counterMutex_);
    ++requestCount_;
}

}