#ifndef STATUS_RPC_H
#define STATUS_RPC_H

#include <string>
#include <grpcpp/grpcpp.h>
#include "status.grpc.pb.h"

namespace zinpass::rpc {

class StatusRPC{
public:
    template <typename T> struct Return {
        T data;
        std::string message;
    };

    explicit StatusRPC(std::shared_ptr<grpc::Channel> channel);

    std::string get_version(const std::string& key) const;
    std::string get_version_build_info(const std::string& key) const;
    std::string get_copyleft(const std::string& key) const;
    std::string get_executable_path(const std::string& key) const;
    std::string get_database_path(const std::string& key) const;
    Return<bool> shutdown(const std::string& key) const;
    Return<bool> restart(const std::string& key) const;

private:
    std::unique_ptr<control::StatusService::StatusService::Stub> stub_;
};

}

#endif
