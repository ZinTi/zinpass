#include "rpc/user/user_service.h"

#include "mgr/account_mgr.h"
#include "mgr/authentication.h"
#include "mgr/user_mgr.h"
#include "mod_session/session_mgr.h"

namespace zinpass::service{
grpc::Status UserServiceImpl::GetUser(ServerContext* context, const user::v1::GetUserReq* request, user::v1::GetUserResp* response) {
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 查询用户信息
    business::UserManager user_manager;
    const auto [data, message] = user_manager.find_by_id(user_id);
    if (std::nullopt == data) {
        response->set_message(message);
        return grpc::Status::OK;
    }

    response->set_message(message);
    response->set_user_id(data->getId());
    response->set_username(data->getUsername());
    response->set_nickname(data->getNickname());
    response->set_created_time(data->getCreatedTime());
    response->set_updated_time(data->getUpdatedTime());
    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::CreateUser(ServerContext* context, const user::v1::CreateUserReq* request, user::v1::CreateUserResp* response) {
    // 1. 反序列化请求
    const std::string& username = request->username();
    const std::string& password = request->password();
    const std::string& nickname = request->nickname();
    // 2. 注册
    if (const auto [user_id, message] = business::UserManager::register_user(username, nickname, password);
        36 == user_id.length()) {  // UUID 长 36
        response->set_id(user_id);
        response->set_message(message);
    } else {
        response->set_id("");
        response->set_message(message);
    }

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::ListUsers(ServerContext* context, const user::v1::ListUsersReq* request, user::v1::ListUsersResp* response){

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::ListUsernames(ServerContext* context, const user::v1::ListUsernamesReq* request, user::v1::ListUsernamesResp* response){
    const auto [data, message] = business::UserManager::list_usernames();
    for(const auto& username : data){
        response->add_usernames(username);
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::UpdateUser(ServerContext* context, const user::v1::UpdateUserReq* request, user::v1::UpdateUserResp* response){
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    business::UserManager user_manager;
    const auto [ret, msg] = user_manager.update_user_by_id(user_id, request->username(), request->nickname());
    response->set_success(ret);
    response->set_message(msg);
    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::UpdateMainKey(ServerContext* context, const user::v1::UpdateMainKeyReq* request, user::v1::UpdateMainKeyResp* response){
    // 1. 验证 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }
    const std::string old_key = request->old_key();
    const std::string new_key = request->new_key();

    // 2. 备份登录密码和账号数据
    // ...

    // 3. 更新登录密码
    if (const bool ret_update1 = business::UserManager::change_passwd(user_id, old_key, new_key);
        !ret_update1) {
        response->set_success(false);
        response->set_message("更新登录密码失败");
        response->set_total_count(0);
        response->set_success_count(0);
        return grpc::Status::OK;
        }

    // 4. 使用新密钥重新加密 account 记录
    const auto [total_count, success_count, failed_list] = business::AccountManager::re_encrypt_password_all(user_id, old_key, new_key);
    response->set_success(true);
    response->set_message("执行重加密操作完毕");
    response->set_total_count(total_count);
    response->set_success_count(success_count);
    for (const auto& failed_id : failed_list) {
        response->add_failed_ids(failed_id);
    }
    return grpc::Status::OK;
}

}