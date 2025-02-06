#ifndef USER_SERVICE_H
#define USER_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "user/v1/user.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{

class UserServiceImpl final : public user::v1::UserService::Service {
public:
    grpc::Status GetUser(ServerContext* context, const user::v1::GetUserReq* request, user::v1::GetUserResp* response) override;
    grpc::Status CreateUser(ServerContext* context, const user::v1::CreateUserReq* request, user::v1::CreateUserResp* response) override;
    grpc::Status ListUsers(ServerContext* context, const user::v1::ListUsersReq* request, user::v1::ListUsersResp* response) override;
    grpc::Status ListUsernames(ServerContext* context, const user::v1::ListUsernamesReq* request, user::v1::ListUsernamesResp* response) override;
    grpc::Status UpdateUser(ServerContext* context, const user::v1::UpdateUserReq* request, user::v1::UpdateUserResp* response) override;
    grpc::Status UpdateMainKey(ServerContext* context, const user::v1::UpdateMainKeyReq* request, user::v1::UpdateMainKeyResp* response) override;

    // Status DeleteUser(user::v1::DeleteUserReq* request);

};

}

#endif //USER_SERVICE_H
