#include "mod_session/session.h"
#include <ostream>
#include <utility>

namespace zinpass::mod_session {

Session::Session() {
    this->id_ = "";
    this->sys_user_id_ = "";
    this->data_ = "";
    this->created_time_ = "";
    this->expires_time_ = "";
    this->accessed_time_ = "";
}

Session::Session(
    const std::string& id,
    const std::string& sys_user_id,
    const std::string& data,
    const std::string& created_time,
    const std::string& expires_time,
    const std::string& accessed_time)
    :
    id_(id),
    sys_user_id_(sys_user_id),
    data_(data),
    created_time_(created_time),
    expires_time_(expires_time),
    accessed_time_(accessed_time) {
}
/*
Session::Session(
    std::string id,
    std::string sys_user_id,
    std::string data,
    std::string created_time,
    std::string expires_time,
    std::string accessed_time){
    this->id_ = std::move(id);
    this->sys_user_id_ = std::move(sys_user_id);
    this->data_ = std::move(data);
    this->created_time_ = std::move(created_time);
    this->expires_time_ = std::move(expires_time);
    this->accessed_time_ = std::move(accessed_time);
}*/

Session::Session(const Session &other) {
    this->id_ = other.id_;
    this->sys_user_id_ = other.sys_user_id_;
    this->data_ = other.data_;
    this->created_time_ = other.created_time_;
    this->expires_time_ = other.expires_time_;
    this->accessed_time_ = other.accessed_time_;
}

Session::~Session() = default;

std::string Session::getId() const { return this->id_; }
std::string Session::getSysUserId() const { return this->sys_user_id_; }
std::string Session::getData() const { return this->data_; }
std::string Session::getCreatedTime() const { return this->created_time_; }
std::string Session::getExpiresTime() const { return this->expires_time_; }
std::string Session::getAccessedTime() const { return this->accessed_time_; }
void Session::setId(const std::string& id){ this->id_ = id; }
void Session::setSysUserId(const std::string& sys_user_id) { this->sys_user_id_ = sys_user_id; }
void Session::setData(const std::string& data) { this->data_ = data; }
void Session::setCreatedTime(const std::string& created_time) { this->created_time_ = created_time; }
void Session::setExpiresTime(const std::string& expires_time) { this->expires_time_ = expires_time; }
void Session::setAccessedTime(const std::string& accessed_time) { this->accessed_time_ = accessed_time; }

void Session::print() const {
    /*
    std::cout << "[ zinpass::mod_session::Session]" << this->id_ << std::endl;
    std::cout << "id_: " << this->id_ << std::endl;
    std::cout << "sys_user_id_: " << this->sys_user_id_ << std::endl;
    std::cout << "data_: " << this->data_ << std::endl;
    std::cout << "created_time_: " << this->created_time_ << std::endl;
    std::cout << "expires_time_: " << this->expires_time_ << std::endl;
    std::cout << "accessed_time_: " << this->accessed_time_ << std::endl;*/
}

} // zinpass::models