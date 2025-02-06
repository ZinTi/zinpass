#include "models/view_account.h"
#include <utility>

namespace zinpass::models{

ViewAccount::ViewAccount() {
    id_ = -1;
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

    phone_ = "";
    email_ = "";
    category_ = "";
}

ViewAccount::ViewAccount(
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

    const std::string& phone,
    const std::string& email,
    const std::string& category){
    this->id_ = id;
    this->username_ = username;
    this->nickname_ = nickname;
    this->sub_account_ = sub_account;
    this->postscript_ = postscript;
    this->platform_name_ = platform_name;
    this->provider_name_ = provider_name;
    this->url_ = url;
    this->hotline_ = hotline;
    this->sys_user_id_ = sys_user_id;
    this->created_time_ = created_time;
    this->updated_time_ = updated_time;

    this->phone_ = phone;
    this->email_ = email;
    this->category_ = category;
}

ViewAccount::ViewAccount(ViewAccount const& other) {
    id_ = other.id_;
    username_ = other.username_;
    nickname_ = other.nickname_;
    sub_account_ = other.sub_account_;
    postscript_ = other.postscript_;
    platform_name_ = other.platform_name_;
    provider_name_ = other.provider_name_;
    url_ = other.url_;
    hotline_ = other.hotline_;
    sys_user_id_ = other.sys_user_id_;
    created_time_ = other.created_time_;
    updated_time_ = other.updated_time_;

    phone_ = other.phone_;
    email_ = other.email_;
    category_ = other.category_;
}

ViewAccount::~ViewAccount() = default;

std::string ViewAccount::getPhone() const {
    return this->phone_;
}

std::string ViewAccount::getEmail() const {
    return this->email_;
}

std::string ViewAccount::getCategory() const {
    return this->category_;
}

void ViewAccount::setPhone(const std::string& phone) {
    this->phone_ = phone;
}

void ViewAccount::setEmail(const std::string& email) {
    this->email_ = email;
}

void ViewAccount::setCategory(const std::string& category) {
    this->category_ = category;
}

void ViewAccount::copy(const ViewAccount& other) {
    id_ = other.id_;
    username_ = other.username_;
    nickname_ = other.nickname_;
    sub_account_ = other.sub_account_;
    postscript_ = other.postscript_;
    platform_name_ = other.platform_name_;
    provider_name_ = other.provider_name_;
    url_ = other.url_;
    hotline_ = other.hotline_;
    sys_user_id_ = other.sys_user_id_;
    created_time_ = other.created_time_;
    updated_time_ = other.updated_time_;

    phone_ = other.phone_;
    email_ = other.email_;
    category_ = other.category_;
}

void ViewAccount::print() const {
    /*
    std::cout << "[ zinpass::models::ViewAccount ]" << std::endl;
    std::cout << "id: " << this->id_ << std::endl;
    std::cout << "username: " << this->username_ << std::endl;
    std::cout << "nickname: " << this->nickname_  << std::endl;
    std::cout << "sub-account: " << this->sub_account_  << std::endl;
    std::cout << "postscript: " << this->postscript_ << std::endl;
    std::cout << "platform-name: " << this->platform_name_ << std::endl;
    std::cout << "provider-name: " << this->provider_name_  << std::endl;
    std::cout << "url: " << this->url_ << std::endl;
    std::cout << "hotline: " << this->hotline_ << std::endl;
    std::cout << "sys-user-id: " << this->sys_user_id_ << std::endl;
    std::cout << "created-time: " << this->created_time_ << std::endl;
    std::cout << "updated-time: " << this->updated_time_  << std::endl;

    std::cout << "phone: " << this->phone_  << std::endl;
    std::cout << "email: " << this->email_ << std::endl;
    std::cout << "category: " << this->category_  << std::endl;
    */
}

}
