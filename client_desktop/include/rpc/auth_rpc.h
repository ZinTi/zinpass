#ifndef AUTH_RPC_H
#define AUTH_RPC_H

#include <string>
#include <map>
#include <grpcpp/grpcpp.h>
#include "auth.grpc.pb.h"

namespace zinpass::rpc{

    class AuthRPC {
    public:
        // 1、constructor, 接受一个gRPC通道的共享指针来初始化存根
        AuthRPC(std::shared_ptr<grpc::Channel> channel);

        // 2、RPC service

        /**
         * @brief login
         * @param username
         * @param password
         * @param captcha_input 图灵测试用户输入的验证码
         * @return session_id + user_id + username + nickname + message
         */
        std::map<std::string, std::string> login(const std::string& username, const std::string& password, const std::string& captcha_input);

        /**
         * @brief logout
         * @param session_id
         * @return message
         */
        std::string logout(const std::string& session_id);

        /**
         * @brief change_main_key
         * @param session_id
         * @param old_password
         * @param new_password
         * @param captcha_input
         * @return
         */
        std::map<std::string, std::string> change_main_key(const std::string& session_id, const std::string& old_password, const std::string& new_password, const std::string& captcha_input);

    private:
        // AuthService服务的存根
        std::unique_ptr<user::v1::AuthService::AuthService::Stub> stub_;
    };
}

#endif // AUTH_RPC_H
