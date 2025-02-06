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
    UserState getUserState() const;

    /**
     * 设置用户状态
     * @param state UserState
     */
    void setUserState(const UserState& state);

    // ========== 应用配置状态组（独立锁）==========

    /**
     * 获取客户端根路径
     */
    std::string getClientRootPath() const;

    /**
     * 设置客户端根路径
     */
    void setClientRootPath(const std::string& path);

    /**
     * 获取应用主题
     */
    std::string getTheme() const;
    /**
     * 设置应用主题
     */
    void setTheme(const std::string& theme);

    /**
     * 获取日志级别
     */
    int getLogLevel() const;

    /**
     * 设置日志级别
     */
    void setLogLevel(int level);

    // ========== 独立高频访问状态（单独锁）==========
    int getReqCount() const;
    void incrementReqCount();

private:
    StateManager() = default;

    // 用户状态组
    mutable std::mutex userMutex_;
    UserState userState_;

    // 应用配置组
    mutable std::mutex configMutex_;
    std::string clientRootPath_;    // 客户端根路径
    std::string currentTheme_ = "light";    // 客户端应用主题
    int logLevel_ = 2;

    // 独立计数器
    mutable std::mutex counterMutex_;
    int requestCount_ = 0;
};

}

#endif //STATE_MANAGER_H
