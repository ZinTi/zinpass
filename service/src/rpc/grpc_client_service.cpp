#include "rpc/grpc_client_service.h"
#include "utils/log_manager.h"


namespace zinpass::service {
gRPCClientService::gRPCClientService(const std::atomic<bool>& is_running, const std::string& host, const int port)
    : is_running_(is_running), address_and_port_(host + ":" + std::to_string(port)){
    //
}

gRPCClientService::gRPCClientService(const std::atomic<bool>& is_running, const std::string& address_and_port)
    : is_running_(is_running), address_and_port_(address_and_port){
    //
}

gRPCClientService::~gRPCClientService() {
    if (server_) {
        server_->Shutdown();
    }
    utils::LogManager::AddLog("[MESSAGE] gRPCClientService Destructor");
}

void gRPCClientService::run() const {
    utils::LogManager::AddLog("[MESSAGE] Server listening on " + this->address_and_port_ );

    grpc::ServerBuilder builder;
    builder.AddListeningPort(this->address_and_port_, grpc::InsecureServerCredentials());
    builder.RegisterService(&user_svc_);
    // builder.RegisterService(&session_svc_);
    builder.RegisterService(&auth_svc_);
    builder.RegisterService(&telecom_svc_);
    builder.RegisterService(&phone_svc_);
    builder.RegisterService(&account_svc_);
    builder.RegisterService(&category_svc_);

    server_ = builder.BuildAndStart();
    if (!server_) {
        utils::LogManager::AddLog("[ERROR] Failed to start server on " + address_and_port_);
        return;
    }

    // 启动服务器等待线程
    std::thread server_thread([this]() {
        server_->Wait();
    });

    // 主循环检查运行状态
    while (is_running_.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 关闭服务器并等待线程结束
    server_->Shutdown();
    server_thread.join();
    server_.reset();

    utils::LogManager::AddLog("[MESSAGE] Server stopped on " + address_and_port_);
}

}