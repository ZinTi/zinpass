#ifndef MOBILE_PHONE_DAO_H
#define MOBILE_PHONE_DAO_H
#include "models/mobile_phone.h"
#include <string>
#include <vector>
#include "common/dao_status.h"
#include "connection_pool.h"

namespace zinpass::repo{

class MobilePhoneDAO {
public:
    explicit MobilePhoneDAO();
    ~MobilePhoneDAO();

    /**
     * @brief 根据id查询目标mobile_phone记录
     * @param id id
     * @param mobile_phone 保存查询到的mobile_phone信息
     * @return 成功与否
     */
    DaoStatus find_by_id(int id, models::MobilePhone& mobile_phone) const;

    /**
     * @brief 根据 phone_number 查询目标 mobile_phone 记录
     * @param phone_number 手机号
     * @param mobile_phone 保存查询到的mobile_phone信息
     * @return 成功与否
     */
    DaoStatus find_by_number(const std::string& phone_number, models::MobilePhone& mobile_phone) const;

    /**
     * @brief 根据 phone_number 查询 phone_id
     * @param phone_number 手机号
     * @return 查询到的 id 主键，如果未查询到则返回负数
     */
    int find_id_by_number(const std::string& phone_number) const;

    /**
     * @brief 查询所有mobile_phone记录
     * @param sys_user_id 筛选条件：当前登录用户
     * @param telecom_operator 电信运营商
     * @param phone_number 手机号
     * @param mobile_phones 保存查询到的 mobile_phone 信息的vector容器
     * @return 成功与否
     */
    DaoStatus find_all(
        const std::string& sys_user_id,
        const std::string& telecom_operator,
        const std::string& phone_number,
        std::vector<models::MobilePhone>& mobile_phones) const;

    /**
     * @brief 查询所有符合条件的phone_number的列表
     * @param sys_user_id 系统账号ID
     * @param phone_numbers 存放所有查询到的phone_number的容器
     * @return 成功与否
     */
    DaoStatus find_list(const std::string& sys_user_id, std::vector<std::string>& phone_numbers) const;

    /**
     * @brief 查询被表 account 引用的次数（记录数量）
     * @param id 删除目标的id
     * @param sys_user_id 用户 id
     * @return 被引用的次数，如果不存在则返回 0，失败则返回负数
     */
    int get_reference_count(int id, const std::string& sys_user_id) const;

    /**
     * @brief 新增MobilePhone记录
     * @param mobile_phone 新MobilePhone记录
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus add(const models::MobilePhone& mobile_phone) const;

    /**
     * @brief 匹配主键 id 和 sys_user_id，更新除id、sys_user_id、created_time以外的手机号信息
     * @param id 主键
     * @param sys_user_id 所属者 ID
     * @param phone_number 手机号
     * @param telecom_operator 电信运营商
     * @param service_password 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param join_time 入网时间
     * @param phone_area 归属地
     * @param postscript 备注
     * @param updated_time 更新时间
     * @return 执行结果
     */
    [[nodiscard]] DaoStatus update(
        int id,
        const std::string& sys_user_id,
        const std::string& phone_number,
        const std::string& telecom_operator,
        const std::string& service_password,
        const std::string& pin,
        const std::string& puk,
        const std::string& join_time,
        const std::string& phone_area,
        const std::string& postscript,
        const std::string& updated_time
        ) const;

    // [[nodiscard]] DaoStatus update(const models::MobilePhone& mobile_phone) const;

    /**
     * @brief 匹配 id 和 sys_user_id，变更新的 sys_user_id
     * @param id 主键
     * @param old_owner_id 旧 sys_user_id
     * @param new_owner_id 新 sys_user_id
     * @param updated_time 更新时间
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus update(
        int id,
        const std::string& old_owner_id,
        const std::string& new_owner_id,
        const std::string& updated_time) const;

    /**
     * @brief 删除 MobilePhone 记录
     * @param id 删除目标的 id
     * @param sys_user_id 用户 id
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus remove(int id, const std::string& sys_user_id) const;

    /**
     * @brief 先将被引用记录的表的外键替换为 replace_id，再删除 id 主键对应的 MobilePhone 记录
     * @param id 删除目标的 phone id
     * @param sys_user_id 用户 id
     * @param replace_id 将被引用的记录的外键替换为 replace_id
     * @param updated_time 更新时间
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus replace_and_remove(int id, const std::string& sys_user_id, int replace_id, const std::string& updated_time) const;

    /**
     * @brief 删除 MobilePhone 记录、级联删除子表引用行 （子表：account）
     * @param id 删除目标的 id
     * @param sys_user_id 用户 id
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus remove_cascade(int id, const std::string& sys_user_id) const;

private:
    ConnectionPool& pool_;

    /**
     * @brief 将 stmt 转换为 MobilePhone，请确保查询列顺序为
     * id
     * phone_number
     * telecom_operator
     * service_password
     * pin
     * puk
     * join_time
     * phone_area
     * postscript
     * sys_user_id
     * created_time
     * updated_time
     * @param stmt
     * @return
     */
    static models::MobilePhone MobilePhoneFromStatement(sqlite3_stmt* stmt);

    /**
     * @brief 生成主键
     * @return 生成的id
     */
    [[nodiscard]] int generate_id() const;
};

}

#endif
