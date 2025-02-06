/**
 * @file session_mgr.h
 * @author zin
 * 单例模式的session管理器
 */
#ifndef SESSION_MGR_H
#define SESSION_MGR_H

#include "session.h"
#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>

namespace zinpass::mod_session {

/**
 * @class SessionMgr
 * @brief Session管理类（单例模式）
 *
 * 负责在内存中管理Session数据，提供增删查改等功能
 */
class SessionMgr {
public:
    /**
     * @brief 获取 SessionMgr 单例实例
     * @return SessionMgr 的静态实例引用
     */
    static SessionMgr& get_instance();

    /**
     * @brief 创建新的Session
     * @param sys_user_id 用户 id
     * @param data Session数据
     * @param expires_seconds 过期时间(秒), 默认 10 分钟
     * @return 新创建Session的ID
     */
    std::string create(
        const std::string& sys_user_id,
        const std::string& data,
        int expires_seconds = 600
        );

    /**
     * @brief 根据 session_id 销毁会话，清除相关数据
     * @param session_id 要删除的Session ID
     * @return 成功返回true，否则false
     */
    bool destroy(const std::string& session_id);

    /**
     * @brief 更新Session字段
     * @param session_id Session ID
     * @param sys_user_id 新的用户 id(可选)
     * @param data 新的数据(可选)
     * @param expires_time 新的过期时间(可选)
     * @param accessed_time 新的访问时间(可选)
     */
    void update(
        const std::string& session_id,
        std::optional<std::string> sys_user_id = std::nullopt,
        std::optional<std::string> data = std::nullopt,
        std::optional<std::string> expires_time = std::nullopt,
        std::optional<std::string> accessed_time = std::nullopt
        );

    /**
     * @brief 刷新Session访问时间
     * @param session_id Session ID
     * @param expires_seconds 新的过期时间(秒)
     */
    void refresh(const std::string& session_id, int expires_seconds);

    /**
     * @brief 获取Session
     * @param session_id Session ID
     * @return 找到的Session对象指针，未找到返回nullptr
     */
    Session* get_session(const std::string& session_id);

    /**
     * @brief 验证Session是否有效
     * @param session_id Session ID
     * @return 有效返回true，否则false
     */
    bool validate_session(const std::string& session_id);

    /**
     * @brief 检查会话是否有效（未过期且存在）
     * @param session_id 会话主键
     * @return 若有效则返回 user_id
     */
    std::string validate_session_and_get_user_id(const std::string &session_id);

    /**
     * @brief 检查会话是否有效（未过期且存在），如果有效则延长会话有效期（可选）并返回 user_id
     * @param session_id 会话主键
     * @param renew 是否延长有效期
     * @return 若有效则返回 user_id
     */
    std::string validate_session_and_get_user_id(const std::string &session_id, bool renew);

    /**
     * @brief 清理过期会话（由定时任务调用）。
     * @return 清除数量
     */
    int purge_expired_sessions(void);

private:
    SessionMgr() = default; // 禁止实例化
    SessionMgr(const SessionMgr&) = delete; // 禁止拷贝
    SessionMgr& operator=(const SessionMgr&) = delete; // 禁止赋值

    std::unordered_map<std::string, Session> sessions_map_;
    std::mutex mutex_;  // 线程安全锁
};

}

#endif

