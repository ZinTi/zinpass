#include "auth_rpc.h"
#include <QDebug>

namespace zinpass::rpc{
    AuthRPC::AuthRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(user::v1::AuthService::AuthService::NewStub(channel)){}

    std::map<std::string, std::string> AuthRPC::login(const std::string& username, const std::string& password, const std::string& captcha_input){
        // 1. Construct request
        user::v1::CreateSessionReq request;
        request.set_username(username);
        request.set_main_key(password);
        request.set_captcha_input(captcha_input);
        // 2. Construct response
        user::v1::CreateSessionResp response;
        // 3. Create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->CreateSession(&context, request, &response);
        if(!status.ok()){
            std::map<std::string, std::string> empty_map;
            return empty_map;
        }
        std::map<std::string, std::string> res{
            {"session_id", response.session_id()},
            {"user_id", response.user_id()},
            {"username", response.username()},
            {"nickname", response.nickname()},
            {"message", response.message()}
        };
        return res;
    }

    std::string AuthRPC::logout(const std::string& session_id){
        // 1. Construct request
        user::v1::DeleteSessionReq request;
        request.set_session_id(session_id);
        // 2. Construct response
        user::v1::DeleteSessionResp response;
        // 3. Create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->DeleteSession(&context, request, &response);
        if(!status.ok()){
            return "RPC failed";
        }
        return response.message();
    }

    std::map<std::string, std::string> AuthRPC::change_main_key(const std::string& session_id, const std::string& old_password, const std::string& new_password, const std::string& captcha_input){
        // 1. Construct request

        // 2. Construct response

        // 3. Create client context

        // 4. Initiate RPC call and fetch status
        std::map<std::string, std::string> empty_map;
        return empty_map;
    }

}
