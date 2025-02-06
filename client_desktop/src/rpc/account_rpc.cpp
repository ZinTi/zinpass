#include "account_rpc.h"
#include <QDebug>
namespace zinpass::rpc{
    AccountRPC::AccountRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(account::v1::AccountService::AccountService::NewStub(channel)){}

    AccountRPC::Return<std::vector<std::string>> AccountRPC::list_email_addresses(const std::string& session_id){
        // 1. construct request
        account::v1::ListEmailAddressesReq request;
        request.set_session_id(session_id);
        // 2. construct response
        account::v1::ListEmailAddressesResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListEmailAddresses(&context, request, &response);
        if (!status.ok()) {
            std::vector<std::string> empty_vector;
            return {empty_vector, response.message()};
        }
        std::vector<std::string> email_addresses;
        for (const auto& email : response.email_addresses()) {
            email_addresses.push_back(email);
        }
        return {email_addresses, response.message()};
    }

    AccountRPC::Return<bool> AccountRPC::add_account(
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
        const std::string& main_key)
    {
        account::v1::CreateAccountReq request;
        request.set_session_id(session_id);
        request.set_password(password);
        request.set_main_key(main_key);
        auto* new_value = new account::v1::Account;
        new_value->set_provider_name(provider_name);
        new_value->set_platform_name(platform_name);
        new_value->set_username(username);
        new_value->set_nickname(nickname);
        new_value->set_sub_account(sub_account);
        new_value->set_mobile_phone(mobile_phone);
        new_value->set_email(email);
        new_value->set_postscript(postscript);
        new_value->set_url(url);
        new_value->set_hotline(hotline);
        new_value->set_category(category);
        request.set_allocated_account(new_value);
        account::v1::CreateAccountResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->CreateAccount(&context, request, &response);
        return {response.result(), response.message()};
    }

    /*
    AccountRPC::Return<std::vector<account::v1::Account>> AccountRPC::list_accounts(
        const std::string& session_id,
        common::v1::Filter* filter,
        int32_t page_size){
        // 1. constructor request
        account::v1::ListAccountsReq request;
        request.set_session_id(session_id);
        request.set_allocated_filter(filter);
        request.set_page_size(page_size);
        // 2. constuctor response
        account::v1::ListAccountsResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListAccounts(&context, request, &response);
        if(!status.ok()){
            std::vector<account::v1::Account> empty_vector;
            return {empty_vector, response.message()};
        }
        std::vector<account::v1::Account> accounts;
        for(const auto& account : response.accounts()){
            accounts.push_back(account);
        }
        return {accounts, response.message()};
    }*/

    AccountRPC::Return<zinpass::models::ViewAccount> AccountRPC::list_account_by_id(const std::string& session_id, const std::string& account_id){
        account::v1::ListAccountByIdReq request;
        request.set_session_id(session_id);
        request.set_account_id(account_id);
        account::v1::ListAccountByIdResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->ListAccountById(&context, request, &response);
        if(!status.ok()){
            models::ViewAccount empty_view_account;
            return {empty_view_account, response.message()};
        }
        models::ViewAccount view_acc;
        view_acc.setId(response.account().account_id());
        view_acc.setProviderName(response.account().provider_name());
        view_acc.setPlatformName(response.account().platform_name());
        view_acc.setUsername(response.account().username());
        view_acc.setNickname(response.account().nickname());
        view_acc.setPhone(response.account().mobile_phone());
        view_acc.setEmail(response.account().email());
        view_acc.setCategory(response.account().category());
        view_acc.setPostscript(response.account().postscript());
        view_acc.setSubAccount(response.account().sub_account());
        view_acc.setURL(response.account().url());
        view_acc.setHotline(response.account().hotline());
        view_acc.setCreatedTime(response.account().created_time());
        view_acc.setUpdatedTime(response.account().updated_time());
        return {view_acc, response.message()};
    }

    AccountRPC::Return<std::vector<zinpass::models::ViewAccount>> AccountRPC::list_accounts(
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
        const int32_t page_size){
        // 1. constructor request
        account::v1::ListAccountsReq request;
        request.set_session_id(session_id);
        auto* conditions = new account::v1::Account();  // 由RPC释放
        conditions->set_provider_name(provider_name);
        conditions->set_platform_name(platform_name);
        conditions->set_username(username);
        conditions->set_nickname(nickname);
        conditions->set_sub_account(sub_account);
        conditions->set_mobile_phone(mobile_phone);
        conditions->set_email(email);
        conditions->set_postscript(postscript);
        conditions->set_url(url);
        conditions->set_hotline(hotline);
        conditions->set_category(category);
        request.set_allocated_conditions(conditions);
        request.set_page_size(page_size);
        // 2. constuctor response
        account::v1::ListAccountsResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListAccounts(&context, request, &response);
        if(!status.ok()){
            std::vector<models::ViewAccount> empty_vector;
            return {empty_vector, response.message()};
        }
        std::vector<models::ViewAccount> accounts;
        for(const auto& res_acc : response.accounts()){
            models::ViewAccount view_acc;
            view_acc.setId(res_acc.account_id());
            view_acc.setProviderName(res_acc.provider_name());
            view_acc.setPlatformName(res_acc.platform_name());
            view_acc.setUsername(res_acc.username());
            view_acc.setNickname(res_acc.nickname());
            view_acc.setPhone(res_acc.mobile_phone());
            view_acc.setEmail(res_acc.email());
            view_acc.setCategory(res_acc.category());
            view_acc.setPostscript(res_acc.postscript());
            view_acc.setSubAccount(res_acc.sub_account());
            view_acc.setURL(res_acc.url());
            view_acc.setHotline(res_acc.hotline());
            view_acc.setCreatedTime(res_acc.created_time());
            view_acc.setUpdatedTime(res_acc.updated_time());
            accounts.emplace_back(view_acc);
        }
        return {accounts, response.message()};
    }

    AccountRPC::Return<bool> AccountRPC::update_account(
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
        const std::string& main_key)
    {
        account::v1::UpdateAccountReq request;
        request.set_session_id(session_id);
        request.set_account_id(account_id);
        request.set_password(account_password);
        request.set_main_key(main_key);
        auto* new_value = new account::v1::Account;
        new_value->set_provider_name(provider_name);
        new_value->set_platform_name(platform_name);
        new_value->set_username(username);
        new_value->set_nickname(nickname);
        new_value->set_sub_account(sub_account);
        new_value->set_mobile_phone(mobile_phone);
        new_value->set_email(email);
        new_value->set_postscript(postscript);
        new_value->set_url(url);
        new_value->set_hotline(hotline);
        new_value->set_category(category);
        request.set_allocated_account(new_value);
        account::v1::UpdateAccountResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->UpdateAccount(&context, request, &response);
        return {response.result(), response.message()};
    }

    AccountRPC::Return<bool> AccountRPC::remove_account(const std::string& session_id, const std::string& account_id, const std::string& main_key){
        account::v1::DeleteAccountReq request;
        request.set_session_id(session_id);
        request.set_account_id(account_id);
        request.set_main_key(main_key);
        account::v1::DeleteAccountResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->DeleteAccount(&context, request, &response);
        return {response.result(), response.message()};
    }

    AccountRPC::Return<std::string> AccountRPC::fetch_password(const std::string& session_id, const std::string& account_id,const std::string& main_key){
        account::v1::FetchPasswordReq request;
        request.set_session_id(session_id);
        request.set_account_id(account_id);
        request.set_main_key(main_key);
        account::v1::FetchPasswordResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->FetchPassword(&context, request, &response);
        return {response.password(), response.message()};
    }

}
