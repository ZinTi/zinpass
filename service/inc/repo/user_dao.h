#ifndef SYSTEM_USER_DAO_H
#define SYSTEM_USER_DAO_H

#include "models/system_user.h"
#include <string>
#include <vector>
extern "C" {
#include <sqlite3.h>
}
#include "common/dao_status.h"
#include "connection_pool.h"

namespace zinpass::repo{

class SystemUserDAO {
public:
    // 构造函数接收数据库路径
    explicit SystemUserDAO();
    ~SystemUserDAO();

    /**
     * @brief 通过主键查询 SystemUser 记录
     * @param id 用户主键
     * @param sys_user 存放查询到的用户信息的容器
     * @return 成功与否
     */
    DaoStatus findById(const std::string& id, models::SystemUser& sys_user) const;

    /**
     * @brief 通过 username 查询 SystemUser 记录
     * @param username 用户名
     * @param sys_user 存放查询到的用户信息的容器
     * @return 成功与否
     */
    DaoStatus findByUsername(const std::string& username, models::SystemUser& sys_user) const;

    /**
     * @brief 获取所有username
     * @param usernames 查询到的 username
     * @return 成功与否
     */
    DaoStatus findUsernames(std::vector<std::string>& usernames) const;

    /**
     * @brief 新增SystemUser记录
     * @param sys_user 新SystemUser记录
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus add(const models::SystemUser& sys_user) const;

    /**
     * @brief 修改SystemUser记录
     * @param sys_user 新SystemUser记录
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus update(const models::SystemUser& sys_user) const;

    /**
     * @brief 删除SystemUser记录
     * @param id 删除目标
     * @return 成功与否
     */
    [[nodiscard]] DaoStatus remove(const std::string& id) const;

private:
    ConnectionPool& pool_;
};

}

#endif
