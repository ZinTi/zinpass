#ifndef HELLO_SERVICE_H
#define HELLO_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "hello/hello.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service {

class GreeterServiceImpl final : public hello::Greeter::Service {
public:
    grpc::Status SayHello(grpc::ServerContext* context, const hello::HelloReq* request, hello::HelloReply* reply) override;

};

}

#endif
