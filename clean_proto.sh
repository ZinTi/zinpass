#!/bin/env bash

# Clean gRPC build files
set -e

PROTOS="./protos"
OUT_SERVER="./apps/server/auto_gen"
OUT_CONTROL="./apps/server_ctl/auto_gen"
OUT_CLIENT="./apps/client_desktop/auto_gen/grpc"

COMMON=(
    -I="$PROTOS"
    -I="/usr/include"
    --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin
)

rm -rf "$OUT_SERVER" "$OUT_CONTROL" "$OUT_CLIENT"
