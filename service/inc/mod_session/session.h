#ifndef SESSION_H
#define SESSION_H

#include <string>

namespace zinpass::mod_session {

class Session {
private:
    std::string id_;             // 主键
    std::string sys_user_id_;      // 用户主键
    std::string data_;           // 数据
    std::string created_time_;    // 创建时间
    std::string expires_time_;    // 过期时间
    std::string accessed_time_;   // 访问时间
public:
    // Constructor && Destructor
    Session();
    Session(
        const std::string& id,
        const std::string& sys_user_id,
        const std::string& data,
        const std::string& created_time,
        const std::string& expires_time,
        const std::string& accessed_time
        );
    /*
    Session(
        std::string id,
        std::string sys_user_id,
        std::string data,
        std::string created_time,
        std::string expires_time,
        std::string accessed_time
        );*/
    Session(const Session& other);
    ~Session();

    // Getter && Setter
    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getSysUserId() const;
    [[nodiscard]] std::string getData() const;
    [[nodiscard]] std::string getCreatedTime() const;
    [[nodiscard]] std::string getExpiresTime() const;
    [[nodiscard]] std::string getAccessedTime() const;
    void setId(const std::string& id);
    void setSysUserId(const std::string& sys_user_id);
    void setData(const std::string& data);
    void setCreatedTime(const std::string& created_time);
    void setExpiresTime(const std::string& expires_time);
    void setAccessedTime(const std::string& accessed_time);

    // print
    void print() const;

};

}

#endif //SESSION_H
