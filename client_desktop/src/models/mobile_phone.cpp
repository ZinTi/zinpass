#include "mobile_phone.h"
#include <iostream>

namespace zinpass::models{

MobilePhone::MobilePhone() {
    this->id_ = 0;
    this->phone_number_ = "";
    this->telecom_operator_ = "";
    this->service_pwd_ = "";
    this->pin_ = "";
    this->puk_ = "";
    this->join_time_ = "";
    this->phone_area_ = "";
    this->postscript_ = "";
    this->sys_user_id_ = "";
    this->created_time_ = "";
    this->updated_time_ = "";
}

MobilePhone::MobilePhone(
    const int id,
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
    const std::string& updated_time){
    id_ = id;
    phone_number_ = phone_number;
    telecom_operator_ = telecom_operator;
    service_pwd_ = service_pwd;
    pin_ = pin;
    puk_ = puk;
    join_time_ = join_time;
    phone_area_ = phone_area;
    postscript_ = postscript;
    sys_user_id_ = sys_user_id;
    created_time_ = created_time;
    updated_time_ = updated_time;
}

MobilePhone::MobilePhone(const MobilePhone& other) {
    this->id_ = other.id_;
    this->phone_number_ = other.phone_number_;
    this->telecom_operator_ = other.telecom_operator_;
    this->service_pwd_ = other.service_pwd_;
    this->pin_ = other.pin_;
    this->puk_ = other.puk_;
    this->join_time_ = other.join_time_;
    this->phone_area_ = other.phone_area_;
    this->postscript_ = other.postscript_;
    this->sys_user_id_ = other.sys_user_id_;
    this->created_time_ = other.created_time_;
    this->updated_time_ = other.updated_time_;
}

MobilePhone::~MobilePhone() = default;

int MobilePhone::getId() const { return this->id_; }
std::string MobilePhone::getPhoneNumber() const { return this->phone_number_; }
std::string MobilePhone::getTelecomOperator() const { return this->telecom_operator_; }
std::string MobilePhone::getServicePwd() const { return this->service_pwd_; }
std::string MobilePhone::getPin() const { return this->pin_; }
std::string MobilePhone::getPuk() const { return this->puk_; }
std::string MobilePhone::getJoinTime() const { return this->join_time_; }
std::string MobilePhone::getPhoneArea() const { return this->phone_area_; }
std::string MobilePhone::getPostscript() const {return this->postscript_; }
std::string MobilePhone::getSysUserId() const { return this->sys_user_id_; }
std::string MobilePhone::getCreatedTime() const { return this->created_time_; }
std::string MobilePhone::getUpdatedTime() const { return this->updated_time_; }
void MobilePhone::setId(const int id) { this->id_ = id; }
void MobilePhone::setPhoneNumber(const std::string& phone_number) { this->phone_number_ = phone_number; }
void MobilePhone::setTelecomOperator(const std::string& phone_operator) { this->telecom_operator_ = phone_operator; }
void MobilePhone::setServicePwd(const std::string& password) { this->service_pwd_ = password; }
void MobilePhone::setPin(const std::string& pin) { this->pin_ = pin; }
void MobilePhone::setPuk(const std::string& puk) { this->puk_ = puk; }
void MobilePhone::setJoinTime(const std::string& join_time) { this->join_time_ = join_time; }
void MobilePhone::setPhoneArea(const std::string& phone_area) { this->phone_area_ = phone_area; }
void MobilePhone::setPostscript(const std::string& postscript) { this->postscript_ = postscript; }
void MobilePhone::setSysUserId(const std::string& sys_user_id) { this->sys_user_id_ = sys_user_id; }
void MobilePhone::setCreatedTime(const std::string& created_time) { this->created_time_ = created_time; }
void MobilePhone::setUpdatedTime(const std::string& updated_time) { this->updated_time_ = updated_time; }

void MobilePhone::copy(const MobilePhone& other) {
    this->id_ = other.id_;
    this->phone_number_ = other.phone_number_;
    this->telecom_operator_ = other.telecom_operator_;
    this->service_pwd_ = other.service_pwd_;
    this->pin_ = other.pin_;
    this->puk_ = other.puk_;
    this->join_time_ = other.join_time_;
    this->phone_area_ = other.phone_area_;
    this->postscript_ = other.postscript_;
    this->sys_user_id_ = other.sys_user_id_;
    this->created_time_ = other.created_time_;
    this->updated_time_ = other.updated_time_;
}
void MobilePhone::print() const {
    std::cout << "[ zinpass::models::MobilePhone ]" << std::endl;
    std::cout << "id: " << this->id_ << std::endl;
    std::cout << "phone_number: " << this->phone_number_ << std::endl;
    std::cout << "telecom_operator: " << this->telecom_operator_  << std::endl;
    std::cout << "service_pwd: " << this->service_pwd_  << std::endl;
    std::cout << "pin: " << this->pin_ << std::endl;
    std::cout << "puk: " << this->puk_ << std::endl;
    std::cout << "join_time: " << this->join_time_  << std::endl;
    std::cout << "postscript: " << this->postscript_  << std::endl;
    std::cout << "sys_user_id: " << this->sys_user_id_ << std::endl;
    std::cout << "created_time: " << this->created_time_ << std::endl;
    std::cout << "updated_time: " << this->updated_time_  << std::endl;
}

}