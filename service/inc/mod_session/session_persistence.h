#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <vector>
#include "session.h"

namespace zinpass::mod_session {

class SessionPersistence {
public:

    /**
     *
     * @param session 持久化会话数据
     * @return 成功与否
     */
    static bool add(const mod_session::Session &session);

    /**
     * @brief 删除持久化存储中的会话数据
     * @param session_id 会话主键
     * @return 完成与否
     */
    static bool remove(const std::string &session_id);

    /**
     * @brief 删除所有持久化存储中的会话数据
     * @return 清除数量
     */
    static int remove_all(void);

    // （五）持久化：本程序中暂时用不到，因为截至目前，session 都是保存在sqlite中的，而不是内存中
    /**
     * @brief 将会话数据持久化到数据库或分布式存储（用于跨服务共享）。
     * @param session_id 会话主键
     * @return 成功与否
     */
    static bool save_session(const std::string& session_id);

    /**
     * @brief 从持久化存储中加载会话（如服务重启后恢复）。
     * @param session_id 会话主键
     * @return 会话数据
     */
    static mod_session::Session load_session(const std::string& session_id);

    // （六）统计与监控
    /**
     * @brief 统计当前活跃会话总数，用于监控系统负载。
     * @return 活跃会话总数
     */
    static int get_active_session_count();

    /**
     * @brief 获取所有会话详情（调试或管理后台用，需注意性能）。
     * @return 所有会话数据
     */
    static std::vector<mod_session::Session> list_all_sessions();

};

}

#endif
