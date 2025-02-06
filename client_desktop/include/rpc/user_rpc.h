#ifndef USER_RPC_H
#define USER_RPC_H

#include <string>
#include <optional>
#include <tuple> // 结构化绑定，避免过度设计类
#include <grpcpp/grpcpp.h>

#include "account.h"
#include "user.grpc.pb.h"

namespace zinpass::rpc{
    template<typename T>
    struct UserRPCReturn{
        T data;
        std::string message;
    };

    class UserRPC {
    public:
        // 1、constructor, 接受一个gRPC通道的共享指针来初始化存根
        UserRPC(std::shared_ptr<grpc::Channel> channel);

        // 2、调用远程服务的公共方法
        /**
         * @brief 查询用户信息
         * @param session_id 会话ID
         * @return 返回元组，包含 user_id + username + nickname + created_at + updated_at， 若查询失败则返回空值
         */
        std::tuple<std::string, std::string, std::string, std::string, std::string> get_user(const std::string &session_id) const;

        /**
         * @brief register_account
         * @param username 用户名
         * @param password 密码
         * @param nickname 昵称
         * @return （ user_id | std::nullopt ) + message
         */
        UserRPCReturn<std::optional<std::string>> register_account(const std::string& username, const std::string& password, const std::string& nickname) const;

        // int list_users(const std::string& session_id, std::vector<zinpass::models::User> users); // 获取用户列表（带分页）

        /**
         * @brief list_usernames
         * @param usernames 用户名
         * @return 0 表示完成
         */
        int list_usernames(std::vector<std::string>& usernames) const;

        /**
         * @brief update_user
         * @param session_id
         * @param username
         * @param nickname
         * @return bool + message
         */
        UserRPCReturn<bool> update_user(const std::string& session_id, const std::string& username, const std::string& nickname) const;

        /**
         * @brief update_main_key
         * @param session_id
         * @param old_key
         * @param new_key
         * @return result + message + total_count + success_count + failed_ids
         */
        std::tuple<bool, std::string, int, int, std::vector<std::string>> update_main_key(const std::string& session_id, const std::string& old_key, const std::string& new_key) const;

        /**
         * @brief delete_user
         * @param session_id
         * @return bool + message
         */
        UserRPCReturn<bool> delete_user(const std::string& session_id);

    private:
        // UserService服务的存根，用于发起RPC调用
        std::unique_ptr<user::v1::UserService::UserService::Stub> stub_;
    };

}

#endif
