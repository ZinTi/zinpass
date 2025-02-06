#include <status_rpc.h>

namespace zinpass::rpc{

StatusRPC::StatusRPC(std::shared_ptr<grpc::Channel> channel)
    : stub_(control::StatusService::StatusService::NewStub(channel)){}

std::string StatusRPC::get_version(const std::string& key) const {
    control::GetStatusCommonReq request;
    request.set_key(key);
    control::GetVersionRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetVersion(&context, request, &response);
    return response.version();
}
std::string StatusRPC::get_version_build_info(const std::string& key) const {
    control::GetStatusCommonReq request;
    request.set_key(key);
    control::GetVersionBuildInfoRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetVersionBuildInfo(&context, request, &response);
    return response.info();
}
std::string StatusRPC::get_copyleft(const std::string& key) const {
    control::GetStatusCommonReq request;
    request.set_key(key);
    control::GetCopyleftRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetCopyleft(&context, request, &response);
    return response.copyleft();
}
std::string StatusRPC::get_executable_path(const std::string& key) const {
    control::GetStatusCommonReq request;
    request.set_key(key);
    control::GetExecutablePathRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetExecutablePath(&context, request, &response);
    return response.path();
}
std::string StatusRPC::get_database_path(const std::string& key) const {
    control::GetStatusCommonReq request;
    request.set_key(key);
    control::GetDatabasePathRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetDatabasePath(&context, request, &response);
    return response.path();
}

StatusRPC::Return<bool> StatusRPC::shutdown(const std::string& key) const {
    control::ShutdownReq request;
    request.set_key(key);
    control::ShutdownRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->Shutdown(&context, request, &response);
    return {response.result(), response.message()};
}

StatusRPC::Return<bool> StatusRPC::restart(const std::string& key) const {
    control::RestartReq request;
    request.set_key(key);
    control::RestartRes response;
    grpc::ClientContext context;
    grpc::Status status = stub_->Restart(&context, request, &response);
    return {response.result(), response.message()};
}

}
