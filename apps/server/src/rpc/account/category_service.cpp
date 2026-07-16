#include "rpc/account/category_service.h"
#include "mod_session/session_mgr.h"
#include "mgr/account_mgr.h"

#define SECONDS_OF_REFRESH 300

namespace zinpass {

    grpc::Status CategoryServiceImpl::ListNameOfCategories(ServerContext* context, const account::v1::ListNameOfCategoriesReq* request, account::v1::ListNameOfCategoriesResp* response) {
        // 1. 检查 session_id 有效性
        auto& session_mgr = zinpass::SessionMgr::instance();
        const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
        if (user_id.empty()){
            response->set_message("无效会话");
            return grpc::Status::OK;
        }
        session_mgr.refresh(request->session_id(), SECONDS_OF_REFRESH); // 自现在起，延长会话有效期，无效会话除外，单位 s

        const auto[categories, message] = zinpass::AccountManager::get_category_options();
        for(const auto& category : categories){
            response->add_categories(category);
        }
        response->set_message(message);
        return grpc::Status::OK;
    }

}