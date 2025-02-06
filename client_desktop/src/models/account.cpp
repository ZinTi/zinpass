#include "account.h"

namespace zinpass::models {
    Account::Account() {
        id_ = "";
        username_ = "";
        nickname_ = "";
        sub_account_ = "";
        postscript_ = "";
        platform_name_ = "";
        provider_name_ = "";
        url_ = "";
        hotline_ = "";
        sys_user_id_ = "";
        created_time_ = "";
        updated_time_ = "";

        // encryptedPwd;
        // iv;
        phone_id_ = -1;
        email_id_ = -1;
        category_id_ = -1;
    }

    Account::Account(
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
        const int phone_id,
        const std::string& email_id,
        const short category_id)
        :
        id_(id),
        username_(username),
        nickname_(nickname),
        sub_account_(sub_account),
        postscript_(postscript),
        platform_name_(platform_name),
        provider_name_(provider_name),
        url_(url),
        hotline_(hotline),
        sys_user_id_(sys_user_id),
        created_time_(created_time),
        updated_time_(updated_time),

        encrypted_pwd_(encrypted_pwd),
        iv_(iv),
        phone_id_(phone_id),
        email_id_(email_id),
        category_id_(category_id) {
    }

    Account::Account(Account const& account) {
        id_ = account.id_;
        username_ = account.username_;
        nickname_ = account.nickname_;
        sub_account_ = account.sub_account_;
        postscript_ = account.postscript_;
        platform_name_ = account.platform_name_;
        provider_name_ = account.provider_name_;
        url_ = account.url_;
        hotline_ = account.hotline_;
        sys_user_id_ = account.sys_user_id_;
        created_time_ = account.created_time_;
        updated_time_ = account.updated_time_;

        encrypted_pwd_ = account.encrypted_pwd_;
        iv_ = account.iv_;
        phone_id_ = account.phone_id_;
        email_id_ = account.email_id_;
        category_id_ = account.category_id_;
    }

    Account::~Account() = default;

    // Getter && Setter
    std::string Account::getId() const { return this->id_; }
    std::string Account::getUsername() const { return this->username_; }
    std::string Account::getNickname() const { return this->nickname_; }
    std::string Account::getSubAccount() const { return this->sub_account_; }
    std::string Account::getPostscript() const { return this->postscript_; }
    std::string Account::getPlatformName() const { return this->platform_name_; }
    std::string Account::getProviderName() const { return this->provider_name_; }
    std::string Account::getURL() const { return this->url_; }
    std::string Account::getHotline() const { return this->hotline_; }
    std::string Account::getSysUserId() const { return this->sys_user_id_; }
    std::string Account::getCreatedTime() const { return this->created_time_; }
    std::string Account::getUpdatedTime() const { return this->updated_time_; }

    const std::vector<unsigned char>& Account::getEncryptedPwd() const{ return this->encrypted_pwd_; }
    const std::vector<unsigned char>& Account::getIv() const{ return this->iv_; }
    short Account::getPhoneId() const { return this->phone_id_; }
    std::string Account::getEmailId() const { return this->email_id_; }
    short Account::getCategoryId() const { return this->category_id_; }

    void Account::setId(const std::string& id) { this->id_ = id; }
    void Account::setUsername(const std::string& username) { this->username_ = username; }
    void Account::setNickname(const std::string& nickname) { this->nickname_ = nickname; }
    void Account::setSubAccount(const std::string& sub_account) { this->sub_account_ = sub_account; }
    void Account::setPostscript(const std::string& postscript) { this->postscript_ = postscript; }
    void Account::setPlatformName(const std::string& platform_name) { this->platform_name_ = platform_name; }
    void Account::setProviderName(const std::string& provider_name) { this->provider_name_ = provider_name; }
    void Account::setURL(const std::string& url) { this->url_ = url; }
    void Account::setHotline(const std::string& hotline) { this->hotline_ = hotline; }
    void Account::setSysUserId(const std::string& sys_user_id) { this->sys_user_id_ = sys_user_id; }
    void Account::setCreatedTime(const std::string& created_time) { this->created_time_ = created_time; }
    void Account::setUpdatedTime(const std::string& updated_time) { this->updated_time_ = updated_time; }

    void Account::setEncryptedPwd(const std::vector<unsigned char>& encrypted_pwd){ this->encrypted_pwd_ = encrypted_pwd; }
    void Account::setIv(const std::vector<unsigned char>& iv){ this->iv_ = iv; }
    void Account::setPhoneId(const int phone_id) { this->phone_id_ = phone_id; }
    void Account::setEmailId(const std::string& email_id) { this->email_id_ = email_id; }
    void Account::setCategoryId(const short category_id) { this->category_id_ = category_id; }
}