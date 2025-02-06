#include "phone_rpc.h"
#include "mobile_phone.h"

namespace zinpass::rpc{
    PhoneRPC::PhoneRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(account::v1::PhoneService::PhoneService::NewStub(channel)){}

    PhoneRPC::Return<bool> PhoneRPC::add_phone(
        const std::string& session_id,
        const std::string& telecom_operator,
        const std::string& phone_area,
        const std::string& phone_number,
        const std::string& pin,
        const std::string& puk,
        const std::string& service_password,
        const std::string& join_time,
        const std::string& postscript){
        account::v1::CreatePhoneReq request;
        request.set_session_id(session_id);
        auto* phone = new account::v1::Phone;
        phone->set_telecom_operator(telecom_operator);
        phone->set_phone_area(phone_area);
        phone->set_phone_number(phone_number);
        phone->set_pin(pin);
        phone->set_puk(puk);
        phone->set_service_pwd(service_password);
        phone->set_join_time(join_time);
        phone->set_postscript(postscript);
        request.set_allocated_phone(phone);
        account::v1::CreatePhoneResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->CreatePhone(&context, request, &response);
        return {response.result(), response.message()};
    }

    PhoneRPC::Return<std::vector<std::string>> PhoneRPC::list_phone_numbers(const std::string& session_id){
        // 1. construct request
        account::v1::ListPhoneNumbersReq request;
        request.set_session_id(session_id);
        // 2. construct response
        account::v1::ListPhoneNumbersResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListPhoneNumbers(&context, request, &response);
        if (!status.ok()) {
            std::vector<std::string> empty_vector;
            return {empty_vector, response.message()};
        }
        std::vector<std::string> phone_numbers;
        for (const auto& number : response.phone_numbers()) {
            phone_numbers.push_back(number);
        }
        return {phone_numbers, response.message()};
    }

    PhoneRPC::Return<std::vector<zinpass::models::MobilePhone>> PhoneRPC::list_phones(const std::string& session_id, const std::string& telecom, const std::string& phone_number){
        // 1. construct request
        account::v1::ListPhoneReq request;
        request.set_session_id(session_id);
        request.set_telecom(telecom);
        request.set_phone_number(phone_number);
        // 2. construct response
        account::v1::ListPhoneResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListPhone(&context, request, &response);

        std::vector<zinpass::models::MobilePhone> mobile_phones;
        if (!status.ok()) {
            return {mobile_phones, response.message()};
        }

        for (const auto& phone : response.phone_numbers()) {
            models::MobilePhone mobile_phone;
            mobile_phone.setId(phone.phone_id());
            mobile_phone.setPhoneNumber(phone.phone_number());
            mobile_phone.setTelecomOperator(phone.telecom_operator());
            mobile_phone.setServicePwd(phone.service_pwd());
            mobile_phone.setPin(phone.pin());
            mobile_phone.setPuk(phone.puk());
            mobile_phone.setJoinTime(phone.join_time());
            mobile_phone.setPhoneArea(phone.phone_area());
            mobile_phone.setPostscript(phone.postscript());
            mobile_phone.setSysUserId(phone.sys_user_id());
            mobile_phone.setCreatedTime(phone.created_time());
            mobile_phone.setUpdatedTime(phone.updated_time());
            mobile_phones.push_back(mobile_phone);
        }
        return {mobile_phones, response.message()};
    }


    PhoneRPC::Return<zinpass::models::MobilePhone> PhoneRPC::find_phone_by_id(const std::string& session_id, const int phone_id){
        // 1. construct request
        account::v1::FindPhoneByIdReq request;
        request.set_session_id(session_id);
        request.set_phone_id(phone_id);
        // 2. construct response
        account::v1::FindPhoneByIdResp response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->FindPhoneById(&context, request, &response);

        zinpass::models::MobilePhone mobile_phone;
        if (!status.ok()) {
            return {mobile_phone, response.message()};
        }

        const auto res_phone = response.phone_number();
        mobile_phone.setId(res_phone.phone_id());
        mobile_phone.setPhoneNumber(res_phone.phone_number());
        mobile_phone.setTelecomOperator(res_phone.telecom_operator());
        mobile_phone.setServicePwd(res_phone.service_pwd());
        mobile_phone.setPin(res_phone.pin());
        mobile_phone.setPuk(res_phone.puk());
        mobile_phone.setJoinTime(res_phone.join_time());
        mobile_phone.setPhoneArea(res_phone.phone_area());
        mobile_phone.setPostscript(res_phone.postscript());
        mobile_phone.setSysUserId(res_phone.sys_user_id());
        mobile_phone.setCreatedTime(res_phone.created_time());
        mobile_phone.setUpdatedTime(res_phone.updated_time());

        return {mobile_phone, response.message()};
    }

    int PhoneRPC::get_reference_count(const std::string& session_id, const int phone_id) {
        account::v1::GetReferenceCountReq request;
        request.set_session_id(session_id);
        request.set_id(phone_id);
        account::v1::GetReferenceCountResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->GetReferenceCount(&context, request, &response);
        return response.reference_count();
    }

    PhoneRPC::Return<bool> PhoneRPC::update_phone_by_id(
        const std::string& session_id,
        const int phone_id,
        const std::string& telecom_operator,
        const std::string& phone_area,
        const std::string& phone_number,
        const std::string& pin,
        const std::string& puk,
        const std::string& service_password,
        const std::string& join_time,
        const std::string& postscript)
    {
        account::v1::UpdatePhoneReq request;
        request.set_session_id(session_id);
        request.set_phone_id(phone_id);
        auto* phone = new account::v1::Phone;
        phone->set_phone_number(phone_number);
        phone->set_telecom_operator(telecom_operator);
        phone->set_service_pwd(service_password);
        phone->set_pin(pin);
        phone->set_puk(puk);
        phone->set_join_time(join_time);
        phone->set_phone_area(phone_area);
        phone->set_postscript(postscript);
        request.set_allocated_phone(phone);
        account::v1::UpdatePhoneResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->UpdatePhone(&context, request, &response);
        return {response.result(), response.message()};
    }

    std::tuple<bool, std::string, int> PhoneRPC::delete_phone_by_id(const std::string& session_id, int phone_id, int mode, int replace_phone_id){
        account::v1::DeletePhoneReq request;
        request.set_session_id(session_id);
        request.set_phone_id(phone_id);
        request.set_mode(mode);
        request.set_replace_phone_id(replace_phone_id);
        account::v1::DeletePhoneResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->DeletePhone(&context, request, &response);
        return {response.result(), response.message(), response.reference_count()};
    }
}
