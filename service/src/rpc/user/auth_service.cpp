#include "rpc/user/auth_service.h"
#include "mgr/authentication.h"

namespace zinpass::service{

grpc::Status AuthServiceImpl::AuthUser(grpc::ServerContext* context, const user::v1::AuthUserReq* request, user::v1::AuthUserResp* response) {

    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::CreateSession(ServerContext* context, const user::v1::CreateSessionReq* request, user::v1::CreateSessionResp* response) {
    // 1. 反序列化请求
    const std::string& username = request->username();
    const std::string& password = request->main_key();
    // const std::string& captcha_input = request->captcha_input();
    // 2. 建立会话
    if(const auto [data, message] = business::Authentication::login_by_username(username, password);
        std::nullopt == data) {
        // Session creation failed
        response->set_message(message);
    } else {
        // Session created successfully
        response->set_session_id(data->session_id);
        response->set_user_id(data->user.getId());
        response->set_username(data->user.getUsername());
        response->set_nickname(data->user.getNickname());
        response->set_message(message);
    }

    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::UpdateSession(ServerContext* context, const user::v1::UpdateSessionReq* request, user::v1::UpdateSessionResp* response) {

    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::DeleteSession(ServerContext* context, const user::v1::DeleteSessionReq* request, user::v1::DeleteSessionResp* response) {

    return grpc::Status::OK;
}

}