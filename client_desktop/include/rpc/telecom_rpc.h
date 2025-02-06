#ifndef TELECOM_RPC_H
#define TELECOM_RPC_H

#include <string>
// #include <map>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "telecom.grpc.pb.h"

namespace zinpass::rpc{

class TelecomRPC{
public:
    template<typename T>
    struct Return{
        T data;
        std::string message;
    };

    TelecomRPC(std::shared_ptr<grpc::Channel> channel);

    /**
     * @brief list_telecom_options
     * @param session_id
     * @return telecom_options + message
     */
    Return<std::vector<std::string>> list_telecom_options(const std::string& session_id);

private:
    std::unique_ptr<account::v1::TelecomService::TelecomService::Stub> stub_; // TelecomService服务的存根
};

}

#endif
