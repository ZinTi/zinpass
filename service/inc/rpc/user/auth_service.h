#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "user/v1/auth.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{
class AuthServiceImpl final : public user::v1::AuthService::Service {
public:
    grpc::Status AuthUser(grpc::ServerContext* context, const user::v1::AuthUserReq* request, user::v1::AuthUserResp* response) override;
    grpc::Status CreateSession(ServerContext* context, const user::v1::CreateSessionReq* request, user::v1::CreateSessionResp* response) override;
    grpc::Status UpdateSession(ServerContext* context, const user::v1::UpdateSessionReq* request, user::v1::UpdateSessionResp* response) override;
    grpc::Status DeleteSession(ServerContext* context, const user::v1::DeleteSessionReq* request, user::v1::DeleteSessionResp* response) override;
};
}

#endif //AUTH_SERVICE_H
