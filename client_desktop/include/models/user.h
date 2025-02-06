#ifndef SYSTEM_USER_H
#define SYSTEM_USER_H

#include <string>

namespace zinpass::models{

class SystemUser {
    std::string id_;             // 主键 UUID
    std::string username_;       // 用户名
    std::string nickname_;       // 昵称
    std::string password_;       // 经哈希后的密码值
    std::string salt_;           // 盐值
    std::string created_time_;    // 创建时间
    std::string updated_time_;    // 更新时间

public:
    // Constructor && Destructor
    SystemUser();
    SystemUser(
        const std::string& id,
        const std::string& username,
        const std::string& nickname,
        const std::string& password,
        const std::string& salt,
        const std::string& created_time,
        const std::string& updated_time
        );
    SystemUser(const SystemUser& other);
    ~SystemUser();

    // Operator Overloading
    SystemUser& operator=(const SystemUser& other);

    // Getter && Setter
    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getUsername() const;
    [[nodiscard]] std::string getNickname() const;
    [[nodiscard]] std::string getPassword() const;
    [[nodiscard]] std::string getSalt() const;
    [[nodiscard]] std::string getCreatedTime() const;
    [[nodiscard]] std::string getUpdatedTime() const;
    void setId(const std::string& id);
    void setUsername(const std::string& username);
    void setNickname(const std::string& nickname);
    void setPassword(const std::string& password);
    void setSalt(const std::string& salt);
    void setCreatedTime(const std::string& created_time);
    void setUpdatedTime(const std::string& updated_time);

    // print
    void print() const;
};

}

#endif
