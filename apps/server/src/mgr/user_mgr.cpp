#include "mgr/user_mgr.h"
#include "repo/user_dao.h"
#include "utils/pwd_utils.h"
#include "utils/date_time.h"
#include "repo/common/dao_status.h"
#include "utils/uuid_generator.h"

namespace zinpass {

UserResult<std::vector<std::string>> UserManager::list_usernames() {
    const zinpass::SystemUserDAO user_dao;
    std::string message;
    std::vector<std::string> sys_usernames;
    if (const zinpass::DaoStatus ret_status = user_dao.findUsernames(sys_usernames);
        zinpass::DaoStatus::Success != ret_status ) {
        message = "发生错误";
    } else {
        message = "成功";
    }
    return {sys_usernames, message};
}

UserResult<std::optional<zinpass::SystemUser>> UserManager::find_by_id(const std::string& id) {
    const zinpass::SystemUserDAO user_dao;
    zinpass::SystemUser system_user;
    if (const zinpass::DaoStatus ret_status = user_dao.findById(id, system_user);
        zinpass::DaoStatus::Success != ret_status) {
        return {std::nullopt, "发生错误"};
    }
    return {system_user, "成功"};
}

UserResult<std::optional<zinpass::SystemUser>> UserManager::find_by_username(const std::string& username) {
    const zinpass::SystemUserDAO user_dao;
    zinpass::SystemUser system_user;
    if (const zinpass::DaoStatus ret_status = user_dao.findByUsername(username, system_user);
        zinpass::DaoStatus::Success != ret_status) {
        return {std::nullopt,  "发生错误"};
    }
    return {system_user,  "成功"};
}

UserResult<std::string> UserManager::register_user(const std::string& username, const std::string& nickname, const std::string& plaintext_password) {
    // 0. 参数数据合法性校验
    if (username.length() < 3 || username.length() > 16) {
        return {"", "用户名长度不合法"};
    }
    if (plaintext_password.length() < 6 || plaintext_password.length() > 36) {
        return {"", "密码长度不合法"};
    }

    // 1. 构造用户数据对象
    const std::string new_salt = zinpass::PwdUtils::generateRandomString();  // 新生成一个 salt
    const std::string hashed_password = zinpass::PwdUtils::digestPassword(plaintext_password, new_salt); // 根据密码和盐值生成一个密码散列值
    if (hashed_password.empty()) { // 为空即失败
        return {"", "故障，请稍后再试"}; // 散列失败
    }
    const std::string now = zinpass::DateTime::getCurrentDateTime();
    const std::string user_id = zinpass::UUIDGenerator::generate();
    const zinpass::SystemUser new_user(
        user_id,        // id 主键
        username,       // username 用户名
        nickname,       // nickname 昵称
        hashed_password,// password 密码散列值
        new_salt,       // salt 盐值
        now,            // 创建时间
        now             // 更新时间
        );

    // 2. 写入
    const zinpass::SystemUserDAO user_dao;
    if (const zinpass::DaoStatus ret_status = user_dao.add(new_user);
        zinpass::DaoStatus::Success != ret_status) {
        return {"", "创建用户失败"};
    }
    return {user_id, "成功"};
}

UserResult<bool> UserManager::remove_user(const std::string& id) {
    const zinpass::SystemUserDAO user_dao;
    if (const zinpass::DaoStatus ret_status = user_dao.remove(id);
        zinpass::DaoStatus::Success != ret_status) {
        return {false, std::string("失败：") + std::to_string(static_cast<int>(ret_status))};
    }
    return {true, std::string("成功删除用户") + id};
}

UserResult<bool> UserManager::update_user_by_id(const std::string& id, const std::string& new_username, const std::string& new_nickname) {
    const zinpass::SystemUserDAO user_dao;
    zinpass::SystemUser system_user;

    // 2. 获取原来的值
    if (const zinpass::DaoStatus ret_status_1 = user_dao.findById(id, system_user);
        zinpass::DaoStatus::Success != ret_status_1) {
        return {false, "获取原值失败"};
        }

    // 3. 更新新值
    system_user.setUsername(new_username);
    system_user.setNickname(new_nickname);
    system_user.setUpdatedTime(zinpass::DateTime::getCurrentDateTime());
    if (const zinpass::DaoStatus ret_status_2 = user_dao.update(system_user);
        zinpass::DaoStatus::Success != ret_status_2) {
        return {false, "更新新值失败"};
        }

    return {true, "更新成功"};
}

bool UserManager::change_passwd(const std::string& id, const std::string& main_key, const std::string& new_main_key) {
    // 0. 创建部分变量或验证参数合法性
    const zinpass::SystemUserDAO user_dao;
    zinpass::SystemUser user;
    const std::string now = zinpass::DateTime::getCurrentDateTime();

    // 1. 查询用户信息并验证用户身份
    if (const zinpass::DaoStatus ret_find = user_dao.findById(id, user);
        zinpass::DaoStatus::Success != ret_find) {
        return false;   // 发生错误
        }
    if (const std::string hashed_old_key = zinpass::PwdUtils::digestPassword(main_key, user.getSalt());
        hashed_old_key != user.getPassword()) {
        return false;    // 用户名或原主密钥不正确
        }

    // 2. 生成新密码散列值、新盐值、更新时间
    const std::string new_salt = zinpass::PwdUtils::generateRandomString(); // 设置密码时生成一个新盐值
    const std::string hashed_new_key = zinpass::PwdUtils::digestPassword(new_main_key, new_salt); // 根据新盐值生成一个新密码密文

    user.setSalt(new_salt);
    user.setPassword(hashed_new_key);
    user.setUpdatedTime(now);

    // 3. 更新用户信息
    if (const zinpass::DaoStatus ret_status = user_dao.update(user);
        zinpass::DaoStatus::Success != ret_status) {
        return false; // 系统错误，修改密码失败
        }

    return true;
}

}