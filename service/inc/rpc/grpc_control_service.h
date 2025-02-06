#ifndef GRPC_CONTROL_SERVICE_H
#define GRPC_CONTROL_SERVICE_H

#include <atomic>
#include <memory>
#include <string>
#include <grpcpp/server.h>
#include "hello_service.h"
#include "status/status_service.h"

namespace zinpass::service {

class gRPCControlService {
public:
    gRPCControlService(std::atomic<bool>& is_running_ref, const std::string& host, int port);
    explicit gRPCControlService(std::atomic<bool>& is_running_ref, const std::string& address_and_port);
    ~gRPCControlService();
    void run() const;

private:
    std::atomic<bool>& is_running_;   // 可修改的引用
    std::string address_and_port_;

    mutable std::unique_ptr<grpc::Server> server_;
    // 服务实例
    mutable GreeterServiceImpl greeter_svc_;
    mutable StatusServiceImpl status_svc_;
};

}

#endif