#include "rpc/account/category_service.h"
#include "mod_session/session_mgr.h"
#include "mgr/account_mgr.h"

namespace zinpass::service {

    grpc::Status CategoryServiceImpl::ListNameOfCategories(ServerContext* context, const account::v1::ListNameOfCategoriesReq* request, account::v1::ListNameOfCategoriesResp* response) {
        // std::cout << "[RPC] ListNameOfCategories request" << std::endl;
        // 1. 检查 session_id 有效性
        auto& session_mgr = mod_session::SessionMgr::get_instance();
        const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
        if (user_id.empty()){

            response->set_message("无效会话");
            return grpc::Status::OK;
        }

        const auto[categories, message] = business::AccountManager::get_category_options();
        for(const auto& category : categories){
            response->add_categories(category);
        }
        response->set_message(message);
        return grpc::Status::OK;
    }

}