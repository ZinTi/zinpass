#include "rpc/status/status_service.h"
#include "config/about.h"
#include "config/state_manager.h"

#define TEMPORARY_CONTROL_KEY    "test-key"

namespace zinpass::service {

StatusServiceImpl::StatusServiceImpl(std::atomic<bool>& is_running_ref)
    : is_running_(is_running_ref) {}

grpc::Status StatusServiceImpl::GetVersion(grpc::ServerContext* context, const control::GetStatusCommonReq* request, control::GetVersionRes* response) {
    const std::string key = request->key();
    if(key != TEMPORARY_CONTROL_KEY){
        response->set_version("");
    }
    const std::string version = config::About::get_version();
    response->set_version(version);
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::GetVersionBuildInfo(grpc::ServerContext* context, const control::GetStatusCommonReq* request, control::GetVersionBuildInfoRes* response) {
    const std::string key = request->key();
    if(key != TEMPORARY_CONTROL_KEY){
        response->set_info("");
    }
    const std::string info = config::About::get_version_build_info();
    response->set_info(info);
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::GetCopyleft(grpc::ServerContext* context, const control::GetStatusCommonReq* request, control::GetCopyleftRes* response) {
    const std::string key = request->key();
    if(key != TEMPORARY_CONTROL_KEY){
        response->set_copyleft("");
    }
    const std::string copyleft = config::About::get_copyleft();
    response->set_copyleft(copyleft);
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::GetExecutablePath(grpc::ServerContext* context, const control::GetStatusCommonReq* request, control::GetExecutablePathRes* response) {
    const std::string key = request->key();
    if(key != TEMPORARY_CONTROL_KEY){
        response->set_path("");
    }
    const std::string path = config::StateManager::instance().get_executable_path();
    response->set_path(path);
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::GetDatabasePath(grpc::ServerContext* context, const control::GetStatusCommonReq* request, control::GetDatabasePathRes* response) {
    const std::string key = request->key();
    if(key != TEMPORARY_CONTROL_KEY){
        response->set_path("");
    }
    const std::string path = config::StateManager::instance().get_main_database_path();
    response->set_path(path);
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::Shutdown(grpc::ServerContext* context, const control::ShutdownReq* request, control::ShutdownRes* response) {
    const std::string key = request->key();
    if(key != TEMPORARY_CONTROL_KEY){
        response->set_result(false);
        response->set_message("密钥错误，拒绝服务");
    }
    is_running_.store(false);  // 修改运行状态
    response->set_result(true);
    response->set_message("接受指令，正在关闭");
    return grpc::Status::OK;
}

grpc::Status StatusServiceImpl::Restart(grpc::ServerContext* context, const control::RestartReq* request, control::RestartRes* response) {
    return grpc::Status::OK;
}

}