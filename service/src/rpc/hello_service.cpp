#include "rpc/hello_service.h"

namespace zinpass::service {
    grpc::Status GreeterServiceImpl::SayHello(
        grpc::ServerContext* context,
        const hello::HelloReq* request,
        hello::HelloReply* reply) {

        const  std::string prefix("Hello, ");
        reply->set_message(prefix + request->name() + "!");
        return grpc::Status::OK;
    }

}