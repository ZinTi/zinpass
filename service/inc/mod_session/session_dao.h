/**
 * @file session_dao.h
 * 这个文件定义了 SessionDAO 类，用于对 session 表（持久化层）进行增删改查操作
 * 使用 SQLite 管理会话数据不安全，攻击者可通过直接读写 SQLite 数据库绕过权限限制
 * 使用内存虽然也不安全，但比直接读写数据库更安全
 */
#ifndef SESSION_DAO_H
#define SESSION_DAO_H
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "repo/common/dao_status.h"
#include "session.h"
#include "repo/connection_pool.h"
extern "C" {
    #include <sqlite3.h>
}

namespace zinpass::mod_session {

class SessionDAO {
public:
    explicit SessionDAO();
    ~SessionDAO();

    /**
     * @brief 根据 session_id 查询目标 session
     * @param id session_id
     * @param session 查询到的 session 信息
     * @return 执行结果
     */
    repo::DaoStatus find_by_id(const std::string& id, mod_session::Session&session) const;

    /**
     * @brief 查询所有 session 数据的 id 和 过期时间
     * @return session_id 和 过期时间的无序键值对
     */
    std::unordered_map<std::string, std::string> find_id_and_expire() const;

    /**
     * @brief 插入 session
     * @param session 新增的 session 信息
     * @return 执行结果
     */
    repo::DaoStatus add(const mod_session::Session& session) const;

    /**
     * @brief 更新 session
     * @param session 更新后的 session 信息
     * @return 执行结果
     */
    repo::DaoStatus update(const mod_session::Session& session) const;

    /**
     * @brief 删除 session
     * @param id 目标 session_id
     * @return 执行结果
     */
    repo::DaoStatus remove(const std::string &id) const;

    /**
     * @brief 删除所有 session
     * @return 删除数量
     */
    int remove_all(void) const;

    /**
     * @brief 刷新会话的过期时间
     * @param id session_id
     * @param expires_date_time 过期时间
     * @return 执行结果
     */
    repo::DaoStatus update_expiration(const std::string& id, const std::string& expires_date_time) const;

private:
    repo::ConnectionPool& pool_;
    
};

}

#endif //SESSION_DAO_H
