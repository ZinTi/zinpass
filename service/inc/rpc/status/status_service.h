#ifndef STATUS_SERVICE_H
#define STATUS_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "control/status.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{

class StatusServiceImpl final : public control::StatusService::Service {
public:
    explicit StatusServiceImpl(std::atomic<bool>& is_running_ref);

    grpc::Status GetVersion(ServerContext* context, const control::GetStatusCommonReq* request, control::GetVersionRes* response) override;
    grpc::Status GetVersionBuildInfo(ServerContext* context, const control::GetStatusCommonReq* request, control::GetVersionBuildInfoRes* response) override;
    grpc::Status GetCopyleft(ServerContext* context, const control::GetStatusCommonReq* request, control::GetCopyleftRes* response) override;
    grpc::Status GetExecutablePath(ServerContext* context, const control::GetStatusCommonReq* request, control::GetExecutablePathRes* response) override;
    grpc::Status GetDatabasePath(ServerContext* context, const control::GetStatusCommonReq* request, control::GetDatabasePathRes* response) override;

    grpc::Status Shutdown(ServerContext* context, const control::ShutdownReq* request, control::ShutdownRes* response) override;
    grpc::Status Restart(ServerContext* context, const control::RestartReq* request, control::RestartRes* response) override;

private:
    std::atomic<bool>& is_running_;  // 绑定主函数的控制标志
};

}

#endif