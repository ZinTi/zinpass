#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "account/v1/account.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service {
class AccountServiceImpl final : public account::v1::AccountService::Service {
public:

    grpc::Status ListEmailAddresses(ServerContext* context, const account::v1::ListEmailAddressesReq* request, account::v1::ListEmailAddressesResp* response) override;

    grpc::Status CreateAccount(ServerContext *context, const account::v1::CreateAccountReq *request, account::v1::CreateAccountResp *response) override;

    grpc::Status ListAccountById(ServerContext* context, const account::v1::ListAccountByIdReq* request, account::v1::ListAccountByIdResp* response) override;

    grpc::Status ListAccounts(ServerContext* context, const account::v1::ListAccountsReq* request, account::v1::ListAccountsResp* response) override;

    grpc::Status UpdateAccount(ServerContext *context, const account::v1::UpdateAccountReq *request, account::v1::UpdateAccountResp *response) override;

    grpc::Status DeleteAccount(ServerContext *context, const account::v1::DeleteAccountReq *request, account::v1::DeleteAccountResp *response) override;

    grpc::Status FetchPassword(ServerContext* context, const account::v1::FetchPasswordReq* request, account::v1::FetchPasswordResp* response) override;

    grpc::Status ChangePassword(ServerContext* context, const account::v1::ChangePasswordReq* request, account::v1::ChangePasswordResp* response) override;

};

}
#endif //ACCOUNT_SERVICE_H
