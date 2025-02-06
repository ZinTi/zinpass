#include "rpc/grpc_control_service.h"
#include "utils/log_manager.h"
#include <chrono>
#include <thread>
#include <grpcpp/server_builder.h>

namespace zinpass::service {

gRPCControlService::gRPCControlService(std::atomic<bool>& is_running_ref, const std::string& host, const int port)
    :
    is_running_(is_running_ref),
    address_and_port_(host + ":" + std::to_string(port)),
    status_svc_(is_running_ref)
{
  //
}

gRPCControlService::gRPCControlService(std::atomic<bool>& is_running_ref, const std::string& address_and_port)
    :
    is_running_(is_running_ref),
    address_and_port_(address_and_port),
    status_svc_(is_running_ref)  // 注入引用到服务实例
{

}

gRPCControlService::~gRPCControlService() {
  if (server_) {
    server_->Shutdown();
  }
  utils::LogManager::AddLog("[MESSAGE] gRPCControlService Destructor");
}

void gRPCControlService::run() const {
  utils::LogManager::AddLog("[MESSAGE] Server listening on " + address_and_port_);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(address_and_port_, grpc::InsecureServerCredentials());
  builder.RegisterService(&greeter_svc_);
  builder.RegisterService(&status_svc_);

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
