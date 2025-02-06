#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <mutex>
#include <string>

namespace zinpass::state{

class StateManager {
public:
    static StateManager& instance();

    // 禁止拷贝构造函数和赋值运算符
    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    // ========== 用户相关状态组（共享一个锁）==========
    typedef struct UserState {
        std::string session_id;
        std::string user_id;
        std::string username;
        std::string nickname;
        std::string role;
    } UserState;

    /**
     * 获取用户状态
     */
    UserState get_user_state() const;

    /**
     * 设置用户状态
     * @param state UserState
     */
    void set_user_state(const UserState& state);

    // ========== 应用配置状态组（独立锁）==========

    /**
     * 获取客户端根路径
     */
    std::string get_client_root_path() const;

    /**
     * 设置客户端根路径
     */
    void set_client_root_path(const std::string& path);

    /**
     * 获取应用主题
     */
    std::string get_theme() const;
    /**
     * 设置应用主题
     */
    void set_theme(const std::string& theme);

    /**
     * 获取日志级别
     */
    int get_log_level() const;

    /**
     * 设置日志级别
     */
    void set_log_level(int level);

    // ========== 独立高频访问状态（单独锁）==========
    int get_req_count() const;
    void increment_req_count();

private:
    StateManager() = default;

    // 用户状态组
    mutable std::mutex user_mutex_;
    UserState user_state_;

    // 应用配置组
    mutable std::mutex config_mutex_;
    std::string client_root_path_;    // 客户端根路径
    std::string client_theme_ = "light";    // 客户端应用主题
    int log_level_ = 2;

    // 独立计数器
    mutable std::mutex counter_mutex_;
    int request_count_ = 0;
};

}

#endif //STATE_MANAGER_H
