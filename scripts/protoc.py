#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script for compiling .proto files to generate gRPC stub code

Command line arguments:
protoc.py <grpc_home_path> [-c COMPONENTS]
python protoc.py <grpc_home_path> [-c COMPONENTS]

Parameter description:
grpc_home_path: GRPC installation directory path
-c/--components: 3-digit binary string to control which components to compile (service/control/client), default 111 (compile all)

Example:
python protoc.py "third_party/grpc-1.71.0-amd64-win-mingw64" -c 111
"""

import os
import subprocess
import argparse
import sys

def validate_components(components):
    """Validate if components parameter is valid"""
    if len(components) != 3:
        raise argparse.ArgumentTypeError("Components must be a 3-digit binary string")
    if any(char not in '01' for char in components):
        raise argparse.ArgumentTypeError("Components must contain only 0s and 1s")
    return components

def create_directory(path):
    """Create directory and handle exceptions"""
    try:
        os.makedirs(path, exist_ok=True)
        print(f"Successfully created directory: {path}")
    except OSError as e:
        print(f"Failed to create directory [{path}]: {e}")
        sys.exit(1)

def run_protoc(protoc_path, arguments, component_name):
    """Execute protoc command and handle results"""
    try:
        result = subprocess.run(
            [protoc_path] + arguments,
            capture_output=True,
            text=True,
            check=True
        )
        print(f"=== {component_name} compilation successful ===")
        print(f"Output directory: {arguments[0].split('=')[1]}")
        if result.stdout:
            print("Standard output:", result.stdout)
        if result.stderr:
            print("Warning messages:", result.stderr)
        return True
    except subprocess.CalledProcessError as e:
        print(f"=== {component_name} compilation failed ===")
        print("Return code:", e.returncode)
        print("Error message:", e.stderr)
        return False

def main():
    # Parse arguments
    parser = argparse.ArgumentParser(description='Compile .proto files to generate gRPC stub code')
    parser.add_argument('grpc_home_path', help='GRPC installation directory path')
    parser.add_argument('-c', '--components',
                        type=validate_components,
                        default='111',
                        help='3-digit binary string to control which components to compile (service/control/client), e.g., 101 means compile service and client')
    args = parser.parse_args()

    # Use parameters
    print(f"gRPC installation directory: {args.grpc_home_path}")
    print(f"Compilation components setting: {args.components} (service:{args.components[0]}, control:{args.components[1]}, client:{args.components[2]})")

    # Get project root directory
    CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
    PROJECT_ROOT = os.path.dirname(CURRENT_DIR)
    print(f"Project root directory: {PROJECT_ROOT}")

    # Set gRPC paths
    GRPC_INSTALL_DIR = args.grpc_home_path

    # Determine executable suffix based on OS
    exe_suffix = ".exe" if os.name == "nt" else ""

    protoc_path = os.path.join(GRPC_INSTALL_DIR, "bin", f"protoc{exe_suffix}")
    grpc_cpp_plugin_path = os.path.join(GRPC_INSTALL_DIR, "bin", f"grpc_cpp_plugin{exe_suffix}")

    # Verify protoc exists
    if not os.path.exists(protoc_path):
        print(f"Error: protoc executable not found: {protoc_path}")
        sys.exit(1)

    # Prepare directory paths
    protos_src_base_dir = os.path.join(PROJECT_ROOT, "protos")
    target_dir_service = os.path.join(PROJECT_ROOT, "service", "auto_generated")
    target_dir_control = os.path.join(PROJECT_ROOT, "control", "auto_generated")
    target_dir_client = os.path.join(PROJECT_ROOT, "client_desktop", "auto_generated", "grpc")

    # Create directories based on component settings
    if args.components[0] == '1':
        create_directory(target_dir_service)
    if args.components[1] == '1':
        create_directory(target_dir_control)
    if args.components[2] == '1':
        create_directory(target_dir_client)

    # Common arguments
    common_args = [
        "-I=" + protos_src_base_dir,
        "-I=" + os.path.join(GRPC_INSTALL_DIR, "include", "google", "protobuf"),
        "--plugin=protoc-gen-grpc=" + grpc_cpp_plugin_path
    ]

    # Define component-specific proto files
    service_protos = [
        os.path.join(protos_src_base_dir, "common/v1/common.proto"),
        os.path.join(protos_src_base_dir, "hello/hello.proto"),
        os.path.join(protos_src_base_dir, "control/status.proto"),
        os.path.join(protos_src_base_dir, "user/v1/user.proto"),
        os.path.join(protos_src_base_dir, "user/v1/auth.proto"),
        os.path.join(protos_src_base_dir, "account/v1/phone.proto"),
        os.path.join(protos_src_base_dir, "account/v1/account.proto"),
        os.path.join(protos_src_base_dir, "account/v1/category.proto"),
        os.path.join(protos_src_base_dir, "account/v1/telecom.proto")
    ]

    control_protos = [
        os.path.join(protos_src_base_dir, "common/v1/common.proto"),
        os.path.join(protos_src_base_dir, "hello/hello.proto"),
        os.path.join(protos_src_base_dir, "control/status.proto")
    ]

    client_protos = [
        os.path.join(protos_src_base_dir, "common/v1/common.proto"),
        os.path.join(protos_src_base_dir, "user/v1/user.proto"),
        os.path.join(protos_src_base_dir, "user/v1/auth.proto"),
        os.path.join(protos_src_base_dir, "account/v1/phone.proto"),
        os.path.join(protos_src_base_dir, "account/v1/account.proto"),
        os.path.join(protos_src_base_dir, "account/v1/category.proto"),
        os.path.join(protos_src_base_dir, "account/v1/telecom.proto")
    ]

    # Execute compilation based on component settings
    success = True

    # 1. Compile Service component
    if args.components[0] == '1':
        service_args = [
                           "--grpc_out=" + target_dir_service,
                           "--cpp_out=" + target_dir_service
                       ] + common_args + service_protos

        if not run_protoc(protoc_path, service_args, "1. Service component"):
            success = False

    # 2. Compile Control component
    if args.components[1] == '1':
        control_args = [
                           "--grpc_out=" + target_dir_control,
                           "--cpp_out=" + target_dir_control
                       ] + common_args + control_protos

        if not run_protoc(protoc_path, control_args, "2. Control component"):
            success = False

    # 3. Compile Client component
    if args.components[2] == '1':
        client_args = [
                          "--grpc_out=" + target_dir_client,
                          "--cpp_out=" + target_dir_client
                      ] + common_args + client_protos

        if not run_protoc(protoc_path, client_args, "3. Client component"):
            success = False

    # Summary results
    if success:
        print("\nAll specified components compiled successfully!")
        sys.exit(0)
    else:
        print("\nSome components failed to compile, please check error messages!")
        sys.exit(1)

if __name__ == "__main__":
    main()
