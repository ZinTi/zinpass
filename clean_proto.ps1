#Requires -Version 7

# Clean gRPC build files
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

# 清理输出目录
Remove-Item -ItemType Directory -Force -Path $OUT_SERVER, $OUT_CONTROL, $OUT_CLIENT | Out-Null
