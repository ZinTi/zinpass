#include "category_rpc.h"

namespace zinpass::rpc{

    CategoryRPC::CategoryRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(account::v1::CategoryService::CategoryService::NewStub(channel)){}

    CategoryRPCReturn<std::vector<std::string>> CategoryRPC::list_account_categories(const std::string& session_id){
        // 1. construct request
        account::v1::ListNameOfCategoriesReq request;
        request.set_session_id(session_id);
        // 2. construct response
        account::v1::ListNameOfCategoriesResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListNameOfCategories(&context, request, &response);
        if (!status.ok()) {
            std::vector<std::string> empty_vector;
            return {empty_vector, response.message()};
        }
        std::vector<std::string> name_of_categories;
        for (const auto& category : response.categories()) {
            name_of_categories.push_back(category);
        }
        return {name_of_categories, response.message()};
    }
}
