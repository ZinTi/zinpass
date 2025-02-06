#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

namespace zinpass::models{

class Account {
protected:
    std::string id_;                                     // 主键
    std::string username_;                       // 登录用户名
    std::string nickname_;                       // 昵称
    std::string sub_account_;                     // 子账号
    std::string postscript_;                     // 备注
    std::string platform_name_;                   // 平台名
    std::string provider_name_;                   // 供应商名
    std::string url_;                            // 链接
    std::string hotline_;                        // 客服热线
    std::string sys_user_id_;                              // 系统用户主键
    std::string created_time_;                    // 创建时间
    std::string updated_time_;                    // 更新时间
private:
    std::vector<unsigned char> encrypted_pwd_;    // 加密后的密码值
    std::vector<unsigned char> iv_;              // 初始化向量IV
    int phone_id_;                              // 手机号主键
    std::string email_id_;                              // 邮箱号主键
    short category_id_;                           // 类别号主键

public:
    // Constructor && Destructor
    Account();
    Account(
        const std::string& id,
        const std::string& username,
        const std::string& nickname,
        const std::string& sub_account,
        const std::string& postscript,
        const std::string& platform_name,
        const std::string& provider_name,
        const std::string& url,
        const std::string& hotline,
        const std::string& sys_user_id,
        const std::string& created_time,
        const std::string& updated_time,

        std::vector<unsigned char> encrypted_pwd,
        std::vector<unsigned char> iv,
        int phone_id,
        const std::string& email_id,
        short category_id
        );
    Account(Account const& account);
    ~Account();

    // Getter && Setter
    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getUsername() const;
    [[nodiscard]] std::string getNickname() const;
    [[nodiscard]] std::string getSubAccount() const;
    [[nodiscard]] std::string getPostscript() const;
    [[nodiscard]] std::string getPlatformName() const;
    [[nodiscard]] std::string getProviderName() const;
    [[nodiscard]] std::string getURL() const;
    [[nodiscard]] std::string getHotline() const;
    [[nodiscard]] std::string getSysUserId() const;
    [[nodiscard]] std::string getCreatedTime() const;
    [[nodiscard]] std::string getUpdatedTime() const;

    [[nodiscard]] const std::vector<unsigned char>& getEncryptedPwd() const;
    [[nodiscard]] const std::vector<unsigned char>& getIv() const;
    [[nodiscard]] short getPhoneId() const;
    [[nodiscard]] std::string getEmailId() const;
    [[nodiscard]] short getCategoryId() const;

    void setId(const std::string& id);
    void setUsername(const std::string& username);
    void setNickname(const std::string& nickname);
    void setSubAccount(const std::string& sub_account);
    void setPostscript(const std::string& postscript);
    void setPlatformName(const std::string& platform_name);
    void setProviderName(const std::string& provider_name);
    void setURL(const std::string& url);
    void setHotline(const std::string& hotline);
    void setSysUserId(const std::string& sys_user_id);
    void setCreatedTime(const std::string& created_time);
    void setUpdatedTime(const std::string& updated_time);

    void setEncryptedPwd(const std::vector<unsigned char>& encrypted_pwd);
    void setIv(const std::vector<unsigned char>& iv);
    void setPhoneId(int phone_id);
    void setEmailId(const std::string& email_id);
    void setCategoryId(short category_id);
};

}

#endif
