#include "rpc/user/auth_service.h"
#include "mgr/authentication.h"

namespace zinpass::service{

grpc::Status AuthServiceImpl::AuthUser(grpc::ServerContext* context, const user::v1::AuthUserReq* request, user::v1::AuthUserResp* response) {
    //std::cout << "[RPC] AuthUser Req: " << request->DebugString() << std::endl;
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::CreateSession(ServerContext* context, const user::v1::CreateSessionReq* request, user::v1::CreateSessionResp* response) {
    //std::cout << "[RPC] CreateSession Req: " << request->DebugString() << std::endl;
    // 1. 反序列化请求
    const std::string& username = request->username();
    const std::string& password = request->main_key();
    // const std::string& captcha_input = request->captcha_input();
    // 2. 建立会话
    if(const auto [data, message] = business::Authentication::login_by_username(username, password);
        std::nullopt == data) {
        // std::cout << "[RPC] Session creation failed" << std::endl;
        response->set_message(message);
    } else {
        // std::cout << "[RPC] Session created successfully, session_id: " << data->session_id << std::endl;
        response->set_session_id(data->session_id);
        response->set_user_id(data->user.getId());
        response->set_username(data->user.getUsername());
        response->set_nickname(data->user.getNickname());
        response->set_message(message);
    }

    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::UpdateSession(ServerContext* context, const user::v1::UpdateSessionReq* request, user::v1::UpdateSessionResp* response) {
    //std::cout << "[RPC] UpdateSession Req: " << request->DebugString() << std::endl;
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::DeleteSession(ServerContext* context, const user::v1::DeleteSessionReq* request, user::v1::DeleteSessionResp* response) {
    //std::cout << "[RPC] DeleteSession Req: " << request->DebugString() << std::endl;
    return grpc::Status::OK;
}

}