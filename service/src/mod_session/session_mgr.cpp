#include "mod_session/session_mgr.h"
#include "utils/uuid_generator.h"
#include "utils/date_time.h"

namespace zinpass::mod_session {

SessionMgr& SessionMgr::get_instance() {
    static SessionMgr instance; // 线程安全的单例初始化
    return instance;
}

std::string SessionMgr::create(
        const std::string& sys_user_id,
        const std::string& data,
        const int expires_seconds) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁

    std::string session_id = utils::UUIDGenerator::generate();
    const std::string now = utils::DateTime::getCurrentDateTime();
    const std::string expires_date_time = utils::DateTime::addSeconds(now, expires_seconds);

    const mod_session::Session new_session(
        session_id,
        sys_user_id,
        data,
        now,
        expires_date_time,
        now
        );

    sessions_map_.emplace(session_id, new_session);
    return session_id;
}

bool SessionMgr::destroy(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    try {
        sessions_map_.erase(session_id);
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}

void SessionMgr::update(
        const std::string& session_id,
        std::optional<std::string> sys_user_id,
        std::optional<std::string> data,
        std::optional<std::string> expires_time,
        std::optional<std::string> accessed_time) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁

    const auto it = sessions_map_.find(session_id);
    if (it == sessions_map_.end()) return;

    if (sys_user_id) {
        it->second.setSysUserId(sys_user_id.value());
    }
    if (data) {
        it->second.setData(data.value());
    }
    if (expires_time) {
        it->second.setExpiresTime(expires_time.value());
    }
    if (accessed_time) {
        it->second.setAccessedTime(accessed_time.value());
    }
}

void SessionMgr::refresh(const std::string& session_id, const int expires_seconds) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    const auto it = sessions_map_.find(session_id);
    if (it == sessions_map_.end()) return;

    const std::string now = utils::DateTime::getCurrentDateTime();
    const std::string new_expires_date_time = utils::DateTime::addSeconds(now, expires_seconds);

    it->second.setAccessedTime(now);
    it->second.setExpiresTime(new_expires_date_time);
}

Session* SessionMgr::get_session(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    const auto it = sessions_map_.find(session_id);
    if (it != sessions_map_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool SessionMgr::validate_session(const std::string& session_id){
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    const auto it = sessions_map_.find(session_id);
    if (it == sessions_map_.end()) return false;

    if(0 >= utils::DateTime::compareDateTimes(
        it->second.getExpiresTime(),
        utils::DateTime::getCurrentDateTime())
        ){
        return false;
    }
    return true;
}

std::string SessionMgr::validate_session_and_get_user_id(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    const auto it = sessions_map_.find(session_id);
    if (it == sessions_map_.end()) return "";

    if(0 >= utils::DateTime::compareDateTimes(
        it->second.getExpiresTime(),
        utils::DateTime::getCurrentDateTime())
        ){
        return "";
    }
    return it->second.getSysUserId();
}

std::string SessionMgr::validate_session_and_get_user_id(const std::string& session_id, const bool renew) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    const auto it = sessions_map_.find(session_id);
    if (it == sessions_map_.end()) return "";

    if(0 >= utils::DateTime::compareDateTimes(
        it->second.getExpiresTime(),
        utils::DateTime::getCurrentDateTime())
        ){
        return "";
        }

    if (renew) {
        constexpr int seconds = 300;
        refresh(session_id, seconds); // 从当前时间开始计算，延长 session 有效期 5min
    }
    return it->second.getSysUserId();
}

int SessionMgr::purge_expired_sessions(void) {
    std::lock_guard<std::mutex> lock(mutex_);  // 线程安全锁
    int sum = 0;
    const std::string now = utils::DateTime::getCurrentDateTime();

    for (auto it = this->sessions_map_.begin(); it != this->sessions_map_.end(); ) {
        if (0 >= utils::DateTime::compareDateTimes(it->second.getExpiresTime(), now)) {
            it = sessions_map_.erase(it);
            sum++;
        } else {
            ++it;
        }
    }
    return sum;
}

}