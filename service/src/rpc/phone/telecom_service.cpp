#include "rpc/phone/telecom_service.h"
#include "mod_session/session_mgr.h"
#include "mgr/telecom_operator_mgr.h"

namespace zinpass::service {

    grpc::Status TelecomServiceImpl::ListTelecomOperator(grpc::ServerContext *context, const account::v1::ListTelecomOperatorReq *request, account::v1::ListTelecomOperatorResp *response) {
        // 1. 检查 session_id 有效性
        auto& session_mgr = mod_session::SessionMgr::instance();
        const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
        if (user_id.empty()){

            response->set_message("无效会话");
            return grpc::Status::OK;
        }

        const auto[telecom_operator_options, message] = business::TelecomOperatorManager::get_telecom_operator_options();
        for(const auto& telecom : telecom_operator_options){
            response->add_telecoms(telecom);
        }
        response->set_message(message);
        return grpc::Status::OK;
    }
}