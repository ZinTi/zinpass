#include "user_rpc.h"
#include <QDebug>
#include <tuple>

namespace zinpass::rpc {

    UserRPC::UserRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(user::v1::UserService::UserService::NewStub(channel)) {}

    std::tuple<std::string, std::string, std::string, std::string, std::string> UserRPC::get_user(const std::string &session_id) const{
        user::v1::GetUserReq req;
        req.set_session_id(session_id);
        user::v1::GetUserResp resp;
        grpc::ClientContext context;
        if (const grpc::Status status = stub_->GetUser(&context, req, &resp);
            !status.ok()) {
            return std::make_tuple("", "", "", "", "");
        }
        return std::make_tuple(
            resp.user_id().c_str(),
            resp.username().c_str(),
            resp.nickname().c_str(),
            resp.created_time().c_str(),
            resp.updated_time().c_str()
            );
    }

    UserRPCReturn<std::optional<std::string>> UserRPC::register_account(const std::string& username, const std::string& password, const std::string& nickname) const {
        // 1. Construct request
        user::v1::CreateUserReq request;
        request.set_username(username);
        request.set_password(password);
        request.set_nickname(nickname);
        // 2. Construct response
        user::v1::CreateUserResp response;
        // 3. Create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->CreateUser(&context, request, &response);
        if(!status.ok()){
            // qDebug() << "RPC failed: " << status.error_message() << "\n";
            return {std::nullopt, "RPC failed"};
        }
        return {response.id(), response.message()};
    }

    int UserRPC::list_usernames(std::vector<std::string>& usernames) const {
        // 1. construct request
        user::v1::ListUsernamesReq request;
        request.set_message("list usernames");
        // 2. construct response
        user::v1::ListUsernamesResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        if (const grpc::Status status = stub_->ListUsernames(&context, request, &response);
            !status.ok()) {
            return -1;
        }
        for (const auto& username : response.usernames()) {
            usernames.push_back(username);
        }
        return 0;
    }

    UserRPCReturn<bool> UserRPC::update_user(const std::string& session_id, const std::string& username, const std::string& nickname) const {
        user::v1::UpdateUserReq request;
        request.set_session_id(session_id);
        request.set_username(username);
        request.set_nickname(nickname);
        user::v1::UpdateUserResp response;
        grpc::ClientContext context;
        if (const grpc::Status status = stub_->UpdateUser(&context, request, &response);
            !status.ok()) {
            return {false, "RPC failed"};
        }
        return {true, response.message()};
    }

    std::tuple<bool, std::string, int, int, std::vector<std::string>> UserRPC::update_main_key(const std::string& session_id, const std::string& old_key, const std::string& new_key) const {
        user::v1::UpdateMainKeyReq request;
        request.set_session_id(session_id);
        request.set_old_key(old_key);
        request.set_new_key(new_key);
        user::v1::UpdateMainKeyResp response;
        grpc::ClientContext context;
        const grpc::Status status = stub_->UpdateMainKey(&context, request, &response);
        std::vector<std::string> failed_ids;
        for (const auto& failed_id : response.failed_ids()) {
            failed_ids.push_back(failed_id);
        }
        return {
            response.success(),
            response.message(),
            response.total_count(),
            response.success_count(),
            failed_ids};
    }

    UserRPCReturn<bool> UserRPC::delete_user(const std::string& session_id) {
        user::v1::DeleteUserReq request;
        request.set_session_id(session_id);
        //


        return {true, "not implemented"};
    }

}
