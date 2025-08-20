#include "models/user.h"
#include <iostream>

namespace zinpass::models{

SystemUser::SystemUser() {
    this->id_ = "";
    this->username_ = "";
    this->nickname_ = "";
    this->password_ = "";
    this->salt_ = "";
    this->created_time_ = "";
    this->updated_time_ = "";
}

SystemUser::SystemUser(
    const std::string& id,
    const std::string& username,
    const std::string& nickname,
    const std::string& password,
    const std::string& salt,
    const std::string& created_time,
    const std::string& updated_time)
{
    id_ = id;
    username_ = username;
    nickname_ = nickname;
    password_ = password;
    salt_ = salt;
    created_time_ = created_time;
    updated_time_ = updated_time;
}

SystemUser::SystemUser(const SystemUser& other) {
    this->id_ = other.id_;
    this->username_ = other.username_;
    this->nickname_ = other.nickname_;
    this->password_ = other.password_;
    this->salt_ = other.salt_;
    this->created_time_ = other.created_time_;
    this->updated_time_ = other.updated_time_;
}

SystemUser::~SystemUser() = default;

SystemUser& SystemUser::operator=(const SystemUser& other) {
    this->id_ = other.id_;
    this->username_ = other.username_;
    this->nickname_ = other.nickname_;
    this->password_ = other.password_;
    this->salt_ = other.salt_;
    this->created_time_ = other.created_time_;
    this->updated_time_ = other.updated_time_;
    return *this;
}


std::string SystemUser::getId() const { return this->id_; }
std::string SystemUser::getUsername() const { return this->username_; }
std::string SystemUser::getNickname() const { return this->nickname_; }
std::string SystemUser::getPassword() const { return this->password_; }
std::string SystemUser::getSalt() const { return this->salt_; }
std::string SystemUser::getCreatedTime() const { return this->created_time_; }
std::string SystemUser::getUpdatedTime() const { return this->updated_time_; }
void SystemUser::setId(const std::string& id) { this->id_ = id; }
void SystemUser::setUsername(const std::string& username) { this->username_ = username; }
void SystemUser::setNickname(const std::string& nickname) { this->nickname_ = nickname; }
void SystemUser::setPassword(const std::string& password) { this->password_ = password; }
void SystemUser::setSalt(const std::string& salt) { this->salt_ = salt; }
void SystemUser::setCreatedTime(const std::string& created_time) { this->created_time_ = created_time; }
void SystemUser::setUpdatedTime(const std::string& updated_time) { this->updated_time_ = updated_time; }

void SystemUser::print() const{
    std::cout << "[ zinpass::models::SystemUser ]" << std::endl;
    std::cout << "id: " << this->id_ << std::endl;
    std::cout << "username: " << this->username_ << std::endl;
    std::cout << "nickname: " << this->nickname_ << std::endl;
    std::cout << "password: " << this->password_ << std::endl;
    std::cout << "salt: " << this->salt_ << std::endl;
    std::cout << "created_time: " << this->created_time_ << std::endl;
    std::cout << "updated_time: " << this->updated_time_ << std::endl;
}

}
