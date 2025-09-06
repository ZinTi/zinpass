#include "rpc/telecom_rpc.h"

namespace zinpass::rpc{

    TelecomRPC::TelecomRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(account::v1::TelecomService::TelecomService::NewStub(channel)){}

    TelecomRPC::Return<std::vector<std::string>> TelecomRPC::list_telecom_options(const std::string& session_id){
        account::v1::ListTelecomOperatorReq request;
        request.set_session_id(session_id);
        account::v1::ListTelecomOperatorResp response;
        grpc::ClientContext context;
        grpc::Status status = stub_->ListTelecomOperator(&context, request, &response);
        if (!status.ok()) {
            std::vector<std::string> empty_vector;
            return {empty_vector, response.message()};
        }
        std::vector<std::string> telecoms;
        for (const auto& telecom : response.telecoms()) {
            telecoms.push_back(telecom);
        }
        return {telecoms, response.message()};
    }

}
