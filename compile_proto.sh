#!/bin/env bash

# Generate gRPC build files
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

mkdir -p "$OUT_SERVER" "$OUT_CONTROL" "$OUT_CLIENT"

protoc "${COMMON[@]}" \
    --grpc_out="$OUT_SERVER" --cpp_out="$OUT_SERVER" \
    "$PROTOS"/common/v1/common.proto \
    "$PROTOS"/hello/hello.proto \
    "$PROTOS"/control/status.proto \
    "$PROTOS"/user/v1/user.proto \
    "$PROTOS"/user/v1/auth.proto \
    "$PROTOS"/account/v1/phone.proto \
    "$PROTOS"/account/v1/account.proto \
    "$PROTOS"/account/v1/category.proto \
    "$PROTOS"/account/v1/telecom.proto

protoc "${COMMON[@]}" \
    --grpc_out="$OUT_CONTROL" --cpp_out="$OUT_CONTROL" \
    "$PROTOS"/common/v1/common.proto \
    "$PROTOS"/hello/hello.proto \
    "$PROTOS"/control/status.proto

protoc "${COMMON[@]}" \
    --grpc_out="$OUT_CLIENT" --cpp_out="$OUT_CLIENT" \
    "$PROTOS"/common/v1/common.proto \
    "$PROTOS"/user/v1/user.proto \
    "$PROTOS"/user/v1/auth.proto \
    "$PROTOS"/account/v1/phone.proto \
    "$PROTOS"/account/v1/account.proto \
    "$PROTOS"/account/v1/category.proto \
    "$PROTOS"/account/v1/telecom.proto
