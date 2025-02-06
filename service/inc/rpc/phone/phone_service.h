#ifndef PHONE_SERVICE_H
#define PHONE_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "account/v1/phone.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{
class PhoneServiceImpl final : public account::v1::PhoneService::Service {
public:
    grpc::Status CreatePhone(ServerContext *context, const account::v1::CreatePhoneReq *request, account::v1::CreatePhoneResp *response) override;

    grpc::Status ListPhoneNumbers(ServerContext* context, const account::v1::ListPhoneNumbersReq* request, account::v1::ListPhoneNumbersResp* response) override;

    grpc::Status ListPhone(ServerContext *context, const account::v1::ListPhoneReq *request, account::v1::ListPhoneResp *response) override;

    grpc::Status FindPhoneById(ServerContext* context, const account::v1::FindPhoneByIdReq* request, account::v1::FindPhoneByIdResp* response) override;

    grpc::Status GetReferenceCount(ServerContext* context, const account::v1::GetReferenceCountReq* request, account::v1::GetReferenceCountResp* response) override;

    grpc::Status UpdatePhone(ServerContext *context, const account::v1::UpdatePhoneReq *request, account::v1::UpdatePhoneResp *response) override;

    grpc::Status DeletePhone(ServerContext *context, const account::v1::DeletePhoneReq *request, account::v1::DeletePhoneResp *response) override;
};

}
#endif
