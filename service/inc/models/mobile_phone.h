#ifndef MOBILE_PHONE_H
#define MOBILE_PHONE_H

#include <string>

namespace zinpass::models{

class MobilePhone {
    int id_;                        // 主键
    std::string phone_number_;      // 手机号码
    std::string telecom_operator_;  // 电信运营商
    std::string service_pwd_;       // 服务密码
    std::string pin_;               // 个人识别码（PIN）
    std::string puk_;               // 个人解锁码（PUK）
    std::string join_time_;         // 注册时间
    std::string phone_area_;        // 归属地
    std::string postscript_;        // 备注
    std::string sys_user_id_;       // 所属系统用户主键
    std::string created_time_;      // 创建时间
    std::string updated_time_;      // 更新时间
public:
    // Constructor && Destructor
    MobilePhone();
    MobilePhone(
        int id,
        const std::string& phone_number,
        const std::string& telecom_operator,
        const std::string& service_pwd,
        const std::string& pin,
        const std::string& puk,
        const std::string& join_time,
        const std::string& phone_area,
        const std::string& postscript,
        const std::string& sys_user_id,
        const std::string& created_time,
        const std::string& updated_time
        );
    MobilePhone(const MobilePhone& other);
    ~MobilePhone();

    // Getter && Setter
    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getPhoneNumber() const;
    [[nodiscard]] std::string getTelecomOperator() const;
    [[nodiscard]] std::string getServicePwd() const;
    [[nodiscard]] std::string getPin() const;
    [[nodiscard]] std::string getPuk() const;
    [[nodiscard]] std::string getJoinTime() const;
    [[nodiscard]] std::string getPhoneArea() const;
    [[nodiscard]] std::string getPostscript() const;
    [[nodiscard]] std::string getSysUserId() const;
    [[nodiscard]] std::string getCreatedTime() const;
    [[nodiscard]] std::string getUpdatedTime() const;
    void setId(int id);
    void setPhoneNumber(const std::string& phone_number);
    void setTelecomOperator(const std::string& phone_operator);
    void setServicePwd(const std::string& password);
    void setPin(const std::string& pin);
    void setPuk(const std::string& puk);
    void setJoinTime(const std::string& join_time);
    void setPhoneArea(const std::string& phone_area);
    void setPostscript(const std::string& postscript);
    void setSysUserId(const std::string& sys_user_id);
    void setCreatedTime(const std::string& created_time);
    void setUpdatedTime(const std::string& updated_time);

    void copy(const MobilePhone& other);
    void print() const;
};

}

#endif
