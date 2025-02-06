#ifndef ACCOUNT_RPC_H
#define ACCOUNT_RPC_H

#include <string>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "account.grpc.pb.h"
#include "view_account.h"

namespace zinpass::rpc{

class AccountRPC{
public:
    template<typename T>
    struct Return{
        T data;
        std::string message;
    };

    // 1、constructor, 接受一个gRPC通道的共享指针来初始化存根
    AccountRPC(std::shared_ptr<grpc::Channel> channel);

    // 2、RPC service
    /**
     * @brief list_email_addresses
     * @param session_id
     * @return email_addresses + message
     */
    Return<std::vector<std::string>> list_email_addresses(const std::string& session_id);

    /**
     * @brief add_account
     * @param session_id
     * @param provider_name
     * @param platform_name
     * @param username
     * @param nickname
     * @param sub_account
     * @param mobile_phone
     * @param email
     * @param postscript
     * @param url
     * @param hotline
     * @param category
     * @param password
     * @param main_key
     * @return
     */
    Return<bool> add_account(
        const std::string& session_id,
        const std::string& provider_name,
        const std::string& platform_name,
        const std::string& username,
        const std::string& nickname,
        const std::string& sub_account,
        const std::string& mobile_phone,
        const std::string& email,
        const std::string& postscript,
        const std::string& url,
        const std::string& hotline,
        const std::string& category,
        const std::string& password,
        const std::string& main_key);

    /**
     * @brief list_account_by_id
     * @param session_id
     * @param account_id
     * @return view_account + message
     */
    Return<zinpass::models::ViewAccount> list_account_by_id(const std::string& session_id, const std::string& account_id);

    /**
     * @brief list_accounts
     * @param session_id
     * @param provider_name
     * @param platform_name
     * @param username
     * @param nickname
     * @param sub_account
     * @param mobile_phone
     * @param email
     * @param postscript
     * @param url
     * @param hotline
     * @param category
     * @param page_size
     * @return view_accounts + message
     */
    Return<std::vector<zinpass::models::ViewAccount>> list_accounts(
        const std::string& session_id,
        const std::string& provider_name,
        const std::string& platform_name,
        const std::string& username,
        const std::string& nickname,
        const std::string& sub_account,
        const std::string& mobile_phone,
        const std::string& email,
        const std::string& postscript,
        const std::string& url,
        const std::string& hotline,
        const std::string& category,
        int32_t page_size);


    /**
     * @brief update_account
     * @param session_id
     * @param account_id
     * @param provider_name
     * @param platform_name
     * @param username
     * @param nickname
     * @param sub_account
     * @param mobile_phone
     * @param email
     * @param postscript
     * @param url
     * @param hotline
     * @param category
     * @param account_password
     * @param main_key
     * @return result + message
     */
    Return<bool> update_account(
        const std::string& session_id,
        const std::string& account_id,
        const std::string& provider_name,
        const std::string& platform_name,
        const std::string& username,
        const std::string& nickname,
        const std::string& sub_account,
        const std::string& mobile_phone,
        const std::string& email,
        const std::string& postscript,
        const std::string& url,
        const std::string& hotline,
        const std::string& category,
        const std::string& account_password,
        const std::string& main_key);

    /**
     * @brief remove_account
     * @param session_id
     * @param account_id
     * @param main_key
     * @return
     */
    Return<bool> remove_account(const std::string& session_id, const std::string& account_id, const std::string& main_key);

    /**
     * @brief fetch_password
     * @param session_id
     * @param account_id
     * @param main_key
     * @return plaintext + message
     */
    Return<std::string> fetch_password(const std::string& session_id, const std::string& account_id, const std::string& main_key);


private:
    // AccountService服务的存根
    std::unique_ptr<account::v1::AccountService::AccountService::Stub> stub_;
};

}

#endif
