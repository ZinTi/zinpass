#ifndef PHONE_MGR_H
#define PHONE_MGR_H

#include <string>
#include <vector>
#include <tuple>
#include "models/mobile_phone.h"

namespace zinpass::business{

class MobilePhoneManager {
public:
    template<typename T>
    struct Return {
        T value;
        std::string message;
    };

    /**
     * @brief get_phone_options
     * @param sys_user_id 手机号所属用户
     * @return phone_numbers + message
     */
    static Return<std::vector<std::string>> get_phone_options(const std::string& sys_user_id);

    /**
     * @brief get_mobile_phones
     * @param sys_user_id 手机号所属用户
     * @param telecom_operator 电信运营商（可选筛选条件）
     * @param phone_number 手机号（可选筛选条件）
     * @return phones + message
     */
    static Return<std::vector<models::MobilePhone>> get_mobile_phones(const std::string& sys_user_id, const std::string& telecom_operator, const std::string& phone_number);

    /**
     * @brief get_phone_by_owner_and_id 这个方法提供给普通用户查询，只能查询自己的手机号，不能查询其他用户的手机号
     * @param sys_user_id 手机号所属用户
     * @param phone_id 手机号 ID
     * @return phone + message
     */
    static Return<models::MobilePhone> get_phone_by_owner_and_id(const std::string& sys_user_id, int phone_id);

    /**
     * @brief get_phone_by_id 这个方法提供给管理员用户查询，查询指定 id 的手机号
     * @param phone_id 手机号 ID
     * @return phone + message
     */
    static Return<models::MobilePhone> get_phone_by_id(int phone_id);

    /**
     * @brief 查询被表 account 引用的次数（记录数量）
     * @param id 删除目标的id
     * @param sys_user_id 用户 id
     * @return 被引用的次数，如果不存在则返回 0，失败则返回负数
     */
    static int get_reference_count(int id, const std::string& sys_user_id);

    /**
     * @brief add_mobile_phone
     * @param sys_user_id 手机号所属用户
     * @param phone 手机号
     * @param telecom_operator 电信运营商
     * @param service_pwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param join_time 入网时间
     * @param phone_area 归属地
     * @param postscript 备注（附言）
     * @return 成功与否 + message
     */
    static Return<bool> add_mobile_phone(
        const std::string& sys_user_id,
        const std::string& phone,
        const std::string& telecom_operator,
        const std::string& service_pwd,
        const std::string& pin,
        const std::string& puk,
        const std::string& join_time,
        const std::string& phone_area,
        const std::string& postscript);

    /**
     * @brief update_mobile_phone
     * @param id id
     * @param owner_id 记录所属人
     * @param phone_number 手机号
     * @param telecom_operator 电信运营商
     * @param service_pwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param join_time 入网时间
     * @param phone_area 归属地
     * @param postscript 备注
     * @return 成功与否 + message
     */
    static Return<bool> update_mobile_phone(
        int id,
        const std::string& owner_id,
        const std::string& phone_number,
        const std::string& telecom_operator,
        const std::string& service_pwd,
        const std::string& pin,
        const std::string& puk,
        const std::string& join_time,
        const std::string& phone_area,
        const std::string& postscript);

    /**
     * @brief 删除目标手机号
     * @param id 删除目标
     * @param user_id 用户 id（如果目标所属user_id与此id不一致，则不予删除）
     * @param mode 0-1-2-3 代表不同的删除模式，数值越大越激进，删除效果越不可控
     * 0 - 若存在子表引用行，则不删除 ;
     * 1 - 若存在子表引用行，批量修改被引用记录的手机号为指定的手机号 ;
     * 2 - 若存在子表引用行，将引用表的外键设置为 NULL ;
     * 3 - 若存在子表引用行，级联删除所有子表引用记录 ;
     * 若查询被引用记录时发生错误，则不会执行删除操作，忽略 mode 值
     * @param replace_phone_id 指定新的手机号 id（仅在 mode=1 时有效）
     * @return 成功与否 + message + 被引用的次数
     */
    static std::tuple<bool, std::string, int> delete_mobile_phone(int id, const std::string& user_id, int mode, int replace_phone_id);
};

}

#endif