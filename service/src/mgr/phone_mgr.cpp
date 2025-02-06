#include "models/mobile_phone.h"
#include "mgr/phone_mgr.h"
#include "repo/account_dao.h"
#include "utils/date_time.h"
#include "repo/phone_dao.h"

namespace zinpass::business{


MobilePhoneManager::Return<std::vector<std::string>> MobilePhoneManager::get_phone_options(const std::string& sys_user_id) {
    std::vector<std::string> phone_numbers;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_list(sys_user_id, phone_numbers);
        repo::DaoStatus::Success != ret_mobile_phone_dao ) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {phone_numbers, message};
}

MobilePhoneManager::Return<std::vector<models::MobilePhone>> MobilePhoneManager::get_mobile_phones(const std::string& sys_user_id, const std::string& telecom_operator, const std::string& phone_number) {
    std::vector<models::MobilePhone> phones;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_all(sys_user_id, telecom_operator, phone_number, phones);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {phones, message};
}

MobilePhoneManager::Return<models::MobilePhone> MobilePhoneManager::get_phone_by_owner_and_id(const std::string& sys_user_id, const int phone_id) {
    const models::MobilePhone phone;
    models::MobilePhone temp_phone;

    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_by_id(phone_id, temp_phone);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        return {phone, "发生错误"};
    }
    if (temp_phone.getSysUserId() != sys_user_id) {
        return {phone, "未查询到符合条件的记录"};  // 仅查询指定用户的手机号，避免其他用户通过 id 查询到
    }
    return {temp_phone, "成功"};
}

MobilePhoneManager::Return<models::MobilePhone> MobilePhoneManager::get_phone_by_id(const int phone_id) {
    models::MobilePhone phone;
    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_by_id(phone_id, phone);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        return {phone, "发生错误"};
    }
    return {phone, "成功"};
}

int MobilePhoneManager::get_reference_count(const int id, const std::string& sys_user_id) {
    models::MobilePhone phone;
    const repo::MobilePhoneDAO mobile_phone_dao;
    return mobile_phone_dao.get_reference_count(id, sys_user_id);
}

MobilePhoneManager::Return<bool> MobilePhoneManager::add_mobile_phone(
    const std::string& sys_user_id,
    const std::string& phone,
    const std::string& telecom_operator,
    const std::string& service_pwd,
    const std::string& pin,
    const std::string& puk,
    const std::string& join_time,
    const std::string& phone_area,
    const std::string& postscript)
{
    bool ret;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;

    models::MobilePhone new_phone;
    new_phone.setPhoneNumber(phone);
    new_phone.setTelecomOperator(telecom_operator);
    new_phone.setServicePwd(service_pwd);
    new_phone.setPin(pin);
    new_phone.setPuk(puk);
    new_phone.setJoinTime(join_time);
    new_phone.setPhoneArea(phone_area);
    new_phone.setPostscript(postscript);
    // new_phone.setId();
    const std::string now = utils::DateTime::getCurrentDateTime();
    new_phone.setCreatedTime(now);
    new_phone.setUpdatedTime(now);
    new_phone.setSysUserId(sys_user_id);

    if (
        const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.add(new_phone);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        message = "发生了错误";
        ret = false;
    }else {
        message = "成功";
        ret = true;
    }

    return {ret, message};
}

MobilePhoneManager::Return<bool> MobilePhoneManager::update_mobile_phone(
    const int id,
    const std::string& owner_id,
    const std::string& phone_number,
    const std::string& telecom_operator,
    const std::string& service_pwd,
    const std::string& pin,
    const std::string& puk,
    const std::string& join_time,
    const std::string& phone_area,
    const std::string& postscript)
{
    bool ret;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    const std::string now = utils::DateTime::getCurrentDateTime();

    const repo::DaoStatus ret_update = mobile_phone_dao.update(id,owner_id,
        phone_number,telecom_operator,service_pwd,pin,puk,
        join_time,phone_area,postscript,now);

    if (
        repo::DaoStatus::Success != ret_update
    ) {
        message = "发生错误";
        ret = false;
    }else {
        message = "成功";
        ret = true;
    }
    return {ret, message};
}

std::tuple<bool, std::string, int> MobilePhoneManager::delete_mobile_phone(const int id, const std::string& user_id, const int mode, const int replace_phone_id) {
    const repo::MobilePhoneDAO mobile_phone_dao;
    const std::string now = utils::DateTime::getCurrentDateTime();

    const int ref_count = mobile_phone_dao.get_reference_count(id, user_id); // 检查被引次数
    if (ref_count < 0) { // 查询被引次数失败
        return {false, "查询被引次数失败", -1}; // 不确定是否有被引用的记录，忽略 mode 选项，返回 -1 作为提示
    }

    if (ref_count == 0) { // 被引次数为 0，任何模式均可以删除
        if (const repo::DaoStatus ret_status = mobile_phone_dao.remove(id, user_id);
            repo::DaoStatus::Success != ret_status) {
            return {false, "执行删除操作失败", ref_count};
        }
        return {true, "成功", ref_count};
    }

    // 被引次数 ref_count > 0，根据 mode 选项执行删除操作
    switch (mode) {
        case 0: { // m0 - 若存在子表引用行，则不删除
            return {false, "存在子表引用，无法删除", ref_count};
        }
        case 1: { // m1 - 若存在子表引用行，批量修改被引用记录的手机号为指定的手机号
            if (const auto ret_rr = mobile_phone_dao.replace_and_remove(id, user_id, replace_phone_id, now);
                repo::DaoStatus::Success == ret_rr) {
                return {true, "在批量更新子表引用行的外键后删除目标成功", ref_count};
            }
            return {false, "更新子表和删除父表目标过程中发生错误", ref_count};
        }
        case 2: { // m2 - 若存在子表引用行，将引用表的外键设置为 NULL
            // 默认策略就是 ON DELETE SET NULL，所以不需要额外操作
            if (const repo::DaoStatus ret_status = mobile_phone_dao.remove(id, user_id);
                repo::DaoStatus::Success != ret_status) {
                return {false, "执行删除操作失败", ref_count};
            }
            return {true, "成功", ref_count};
        }
        case 3: { // m3 - 若存在子表引用行，级联删除所有子表引用记录
            if (const repo::DaoStatus ret_status = mobile_phone_dao.remove_cascade(id, user_id);
                repo::DaoStatus::Success != ret_status) {
                return {false, "执行删除操作失败", ref_count};
            }
            return {true, "成功", ref_count};
        }
        default: { // 无效 mode 选项，返回错误信息
            return {false, "无效的 mode 选项", ref_count};
        }
    }
}

}