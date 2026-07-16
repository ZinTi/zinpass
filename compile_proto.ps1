#Requires -Version 7

# Generate gRPC build files
$ErrorActionPreference = "Stop"

$PROTOS = "./protos"
$OUT_SERVER = "./apps/server/auto_gen"
$OUT_CONTROL = "./apps/server_ctl/auto_gen"
$OUT_CLIENT = "./apps/client_desktop/auto_gen/grpc"

# protoc 公共参数
$COMMON = @(
    "-I=$PROTOS",
    "-I=C:/include",   # 对应 /usr/include
    "--plugin=protoc-gen-grpc=C:/path/to/grpc_cpp_plugin.exe"
)

# 创建输出目录（如果不存在）
New-Item -ItemType Directory -Force -Path $OUT_SERVER, $OUT_CONTROL, $OUT_CLIENT | Out-Null

# Server + Control + Client 共用 proto
$ALL_PROTO_FILES = @(
    "$PROTOS/common/v1/common.proto",
    "$PROTOS/hello/hello.proto",
    "$PROTOS/control/status.proto",
    "$PROTOS/user/v1/user.proto",
    "$PROTOS/user/v1/auth.proto",
    "$PROTOS/account/v1/phone.proto",
    "$PROTOS/account/v1/account.proto",
    "$PROTOS/account/v1/category.proto",
    "$PROTOS/account/v1/telecom.proto"
)

# Server
protoc @COMMON `
    --grpc_out=$OUT_SERVER `
    --cpp_out=$OUT_SERVER `
    @($ALL_PROTO_FILES)

# Control
protoc @COMMON `
    --grpc_out=$OUT_CONTROL `
    --cpp_out=$OUT_CONTROL `
    "$PROTOS/common/v1/common.proto" `
    "$PROTOS/hello/hello.proto" `
    "$PROTOS/control/status.proto"

# Client
protoc @COMMON `
    --grpc_out=$OUT_CLIENT `
    --cpp_out=$OUT_CLIENT `
    "$PROTOS/common/v1/common.proto" `
    "$PROTOS/user/v1/user.proto" `
    "$PROTOS/user/v1/auth.proto" `
    "$PROTOS/account/v1/phone.proto" `
    "$PROTOS/account/v1/account.proto" `
    "$PROTOS/account/v1/category.proto" `
    "$PROTOS/account/v1/telecom.proto"
