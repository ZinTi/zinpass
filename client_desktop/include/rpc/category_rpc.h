#ifndef CATEGORY_RPC_H
#define CATEGORY_RPC_H

#include <string>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "account/v1/category.grpc.pb.h"

namespace zinpass::rpc{

template<typename T>
struct CategoryRPCReturn{
    T data;
    std::string message;
};

class CategoryRPC{
public:
    // 1. constructor, 接受一个gRPC通道的共享指针来初始化存根
    CategoryRPC(std::shared_ptr<grpc::Channel> channel);

    // 2. RPC service
    /**
     * @brief list_account_categories
     * @param session_id
     * @return name of categories + message
     */
    CategoryRPCReturn<std::vector<std::string>> list_account_categories(const std::string& session_id);

private:
    std::unique_ptr<account::v1::CategoryService::CategoryService::Stub> stub_;
};

}

#endif

