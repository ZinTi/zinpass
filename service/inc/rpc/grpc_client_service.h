#ifndef GRPC_CLIENT_SERVICE_H
#define GRPC_CLIENT_SERVICE_H
#include <atomic>
#include <memory>
#include <string>
#include <grpcpp/server.h>
#include "user/user_service.h"
#include "user/auth_service.h"
#include "phone/telecom_service.h"
#include "phone/phone_service.h"
#include "account/account_service.h"
#include "account/category_service.h"

namespace zinpass::service {

class gRPCClientService {
public:
    gRPCClientService(const std::atomic<bool>& is_running, const std::string& host, int port);
    explicit gRPCClientService(const std::atomic<bool>& is_running, const std::string& address_and_port);
    ~gRPCClientService();
    void run() const;

private:
    const std::atomic<bool>& is_running_;
    std::string address_and_port_;

    mutable std::unique_ptr<grpc::Server> server_;

    mutable UserServiceImpl user_svc_;
    // mutable SessionServiceImpl session_svc_;
    mutable AuthServiceImpl auth_svc_;
    mutable TelecomServiceImpl telecom_svc_;
    mutable PhoneServiceImpl phone_svc_;
    mutable AccountServiceImpl account_svc_;
    mutable CategoryServiceImpl category_svc_;
};

}

#endif
