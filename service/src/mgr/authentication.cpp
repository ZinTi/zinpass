#include "mgr/authentication.h"
#include <sstream>
#include "mgr/account_mgr.h"
#include "repo/user_dao.h"
#include "mgr/user_mgr.h"
#include "mod_session/session_mgr.h"
#include "utils/pwd_utils.h"


namespace zinpass::business {
    AuthResult<std::optional<login_data_t>> Authentication::login_by_id(const std::string& id, const std::string& password) {
        // 0. 创建部分变量并验证参数合法性

        // 1. 查询用户信息并验证用户身份合法性
        const auto [data, message] = UserManager::find_by_id(id);
        if (!data) { // 查询用户失败
            return {std::nullopt, message};
        }

        if(const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {std::nullopt, "用户或密码不正确"};
            }

        // 2. 创建会话
        auto& session_mgr = mod_session::SessionMgr::get_instance();
        const std::string session_id = session_mgr.create(data->getId(), "default data", 600);
        if (session_id.empty()) {
            return {std::nullopt, "会话建立失败"};
        }

        // 3. 返回用户数据和会话ID
        login_data_t success_data;
        success_data.user = *data;    // 解引用并使用重载赋值运算符复制给左值
        success_data.session_id = session_id;
        return {success_data, "登录成功"};
    }

    AuthResult<std::optional<login_data_t>> Authentication::login_by_username(const std::string& username, const std::string& password) {
        // 0. 创建部分变量并验证参数合法性

        // 1. 查询用户信息并验证用户身份合法性
        const auto [data, message] = business::UserManager::find_by_username(username);
        if (!data) { // 查询用户失败
            return {std::nullopt, message};
        }

        if(const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {std::nullopt, "用户或密码不正确"};
        }

        // 2. 创建会话
        auto& session_mgr = mod_session::SessionMgr::get_instance();
        const std::string session_id = session_mgr.create(data->getId(), "default data", 600);
        if (session_id.empty()) {
            return {std::nullopt, "会话建立失败"};
        }

        // 3. 返回用户数据和会话ID
        login_data_t success_data;
        success_data.user = *data;    // 解引用并使用重载赋值运算符复制给左值
        success_data.session_id = session_id;
        return {success_data, "登录成功"};
    }

    AuthResult<bool> Authentication::validate_user_by_id(const std::string& id, const std::string& password) {
        // 0. 创建部分变量并验证参数合法性

        // 1. 查询用户
        const auto [data, message] = business::UserManager::find_by_id(id);
        if (!data) {
            return {false, message};    // 获取用户信息失败
        }

        if (const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {false, "用户或密码不正确"};
        }
        return {true, "验证成功"};
    }

    AuthResult<bool> Authentication::validate_user_by_username(const std::string& username, const std::string& password) {
        // 0. 创建部分变量并验证参数合法性

        // 1. 查询用户
        const auto [data, message] = UserManager::find_by_username(username);
        if (!data) {
            return {false, message};    // 获取用户信息失败
        }

        if (const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {false, "用户名或密码不正确"};
            }
        return {true, "验证成功"};
    }

}