#include "rpc/phone/phone_service.h"
#include "mod_session/session_mgr.h"
#include "mgr/phone_mgr.h"

namespace zinpass::service{

grpc::Status PhoneServiceImpl::CreatePhone(ServerContext *context, const account::v1::CreatePhoneReq *request, account::v1::CreatePhoneResp *response) {
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 新增手机号
    const auto[result, message] = business::MobilePhoneManager::add_mobile_phone(
        user_id,
        request->phone().phone_number(),
        request->phone().telecom_operator(),
        request->phone().service_pwd(),
        request->phone().pin(),
        request->phone().puk(),
        request->phone().join_time(),
        request->phone().phone_area(),
        request->phone().postscript()
        );
    response->set_result(result);
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::ListPhoneNumbers(ServerContext* context, const account::v1::ListPhoneNumbersReq* request, account::v1::ListPhoneNumbersResp* response){
    // std::cout << "[RPC] ListPhoneNumbers Req: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    const auto[phone_numbers, message] = business::MobilePhoneManager::get_phone_options(user_id);
    for(const auto& phone_number : phone_numbers){
        response->add_phone_numbers(phone_number);
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::ListPhone(ServerContext *context, const account::v1::ListPhoneReq *request, account::v1::ListPhoneResp *response) {
    // std::cout << "[RPC] ListPhone Req: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    const std::string& req_telecom_operator = request->telecom();
    const std::string& req_phone_number = request->phone_number();
    const auto[phone_numbers, message] = business::MobilePhoneManager::get_mobile_phones(
        user_id,
        req_telecom_operator,
        req_phone_number
    );
    for (const auto& phone_number : phone_numbers) {
        account::v1::Phone* phone = response->add_phone_numbers();
        phone->set_phone_id(phone_number.getId());
        phone->set_phone_number(phone_number.getPhoneNumber());
        phone->set_telecom_operator(phone_number.getTelecomOperator());
        phone->set_service_pwd(phone_number.getServicePwd());
        phone->set_pin(phone_number.getPin());
        phone->set_puk(phone_number.getPuk());
        phone->set_join_time(phone_number.getJoinTime());
        phone->set_phone_area(phone_number.getPhoneArea());
        phone->set_postscript(phone_number.getPostscript());
        phone->set_sys_user_id(phone_number.getSysUserId());
        phone->set_created_time(phone_number.getCreatedTime());
        phone->set_updated_time(phone_number.getUpdatedTime());
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::FindPhoneById(ServerContext* context, const account::v1::FindPhoneByIdReq* request, account::v1::FindPhoneByIdResp* response) {
    // std::cout << "[RPC] FindPhoneById Req: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }
    const auto[data, message] = business::MobilePhoneManager::get_phone_by_owner_and_id(
        user_id,
        request->phone_id()
    );

    account::v1::Phone* phone = new account::v1::Phone();
    phone->set_phone_id(data.getId());
    phone->set_phone_number(data.getPhoneNumber());
    phone->set_telecom_operator(data.getTelecomOperator());
    phone->set_service_pwd(data.getServicePwd());
    phone->set_pin(data.getPin());
    phone->set_puk(data.getPuk());
    phone->set_join_time(data.getJoinTime());
    phone->set_phone_area(data.getPhoneArea());
    phone->set_postscript(data.getPostscript());
    phone->set_sys_user_id(data.getSysUserId());
    phone->set_created_time(data.getCreatedTime());
    phone->set_updated_time(data.getUpdatedTime());
    response->set_allocated_phone_number(phone);
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::GetReferenceCount(ServerContext* context, const account::v1::GetReferenceCountReq* request, account::v1::GetReferenceCountResp* response) {
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_reference_count(-1);
        return grpc::Status::OK;
    }

    const auto ref_count = business::MobilePhoneManager::get_reference_count(request->id(), user_id);
    response->set_reference_count(ref_count);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::UpdatePhone(ServerContext *context, const account::v1::UpdatePhoneReq *request, account::v1::UpdatePhoneResp *response) {
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 更新 Phone
    const auto[result, message] = business::MobilePhoneManager::update_mobile_phone(
        request->phone_id(),
        user_id,
        request->phone().phone_number(),
        request->phone().telecom_operator(),
        request->phone().service_pwd(),
        request->phone().pin(),
        request->phone().puk(),
        request->phone().join_time(),
        request->phone().phone_area(),
        request->phone().postscript()
        );
    response->set_result(result);
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::DeletePhone(ServerContext *context, const account::v1::DeletePhoneReq *request, account::v1::DeletePhoneResp *response) {
    // 1. 检查 session_id 有效性
    auto& session_mgr = mod_session::SessionMgr::get_instance();
    const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 删除 Phone
    const int mode = request->mode();
    const int replace_phone_id = request->replace_phone_id();
    const auto[result, message, ref_count] = business::MobilePhoneManager::delete_mobile_phone(request->phone_id(), user_id, mode, replace_phone_id);
    response->set_result(result);
    response->set_message(message);
    response->set_reference_count(ref_count);
    return grpc::Status::OK;
}

}