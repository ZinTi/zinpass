"""
编译 .proto 文件生成 gRPC 桩代码的脚本（Linux系统专用）

命令行参数：
python protoc.py [-c COMPONENTS]

参数说明：
-c/--components: 三位二进制字符串，控制要编译的组件（service/control/client），默认111（全部编译）

例如：
python protoc.py -c 101
"""

import os
import subprocess
import argparse
import sys

def validate_components(components):
    """验证components参数是否合法"""
    if len(components) != 3:
        raise argparse.ArgumentTypeError("Components must be a 3-digit binary string")
    if any(char not in '01' for char in components):
        raise argparse.ArgumentTypeError("Components must contain only 0s and 1s")
    return components

def create_directory(path):
    """创建目录并处理异常"""
    try:
        os.makedirs(path, exist_ok=True)
        print(f"成功创建目录: {path}")
    except OSError as e:
        print(f"创建目录失败 [{path}]: {e}")
        sys.exit(1)

def run_protoc(protoc_path, arguments, component_name):
    """执行protoc命令并处理结果"""
    try:
        result = subprocess.run(
            [protoc_path] + arguments,
            capture_output=True,
            text=True,
            check=True
        )
        print(f"=== {component_name} 编译成功 ===")
        print(f"输出目录: {arguments[0].split('=')[1]}")
        if result.stdout:
            print("标准输出:", result.stdout)
        if result.stderr:
            print("警告信息:", result.stderr)
        return True
    except subprocess.CalledProcessError as e:
        print(f"=== {component_name} 编译失败 ===")
        print("返回码:", e.returncode)
        print("错误信息:", e.stderr)
        return False

def main():
    # 解析参数
    parser = argparse.ArgumentParser(description='编译.proto文件生成gRPC桩代码（Linux系统专用）')
    parser.add_argument('-c', '--components',
                        type=validate_components,
                        default='111',
                        help='三位二进制控制要编译的组件（service/control/client），例如101表示编译service和client')
    args = parser.parse_args()

    print(f"编译组件设置: {args.components} (service:{args.components[0]}, control:{args.components[1]}, client:{args.components[2]})")

    # 获取项目根目录
    CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
    PROJECT_ROOT = os.path.dirname(CURRENT_DIR)
    print(f"项目根目录: {PROJECT_ROOT}")

    # 使用Linux标准路径
    protoc_path = "/usr/bin/protoc"
    grpc_cpp_plugin_path = "/usr/bin/grpc_cpp_plugin"

    # 验证protoc是否存在
    if not os.path.exists(protoc_path):
        print(f"错误: 未找到protoc可执行文件: {protoc_path}")
        sys.exit(1)
    if not os.path.exists(grpc_cpp_plugin_path):
        print(f"错误: 未找到grpc_cpp_plugin可执行文件: {grpc_cpp_plugin_path}")
        sys.exit(1)

    # 准备目录路径
    protos_src_base_dir = os.path.join(PROJECT_ROOT, "protos")
    target_dir_service = os.path.join(PROJECT_ROOT, "service", "auto_generated")
    target_dir_control = os.path.join(PROJECT_ROOT, "control", "auto_generated")
    target_dir_client = os.path.join(PROJECT_ROOT, "client_desktop", "auto_generated", "grpc")

    # 根据组件设置创建目录
    if args.components[0] == '1':
        create_directory(target_dir_service)
    if args.components[1] == '1':
        create_directory(target_dir_control)
    if args.components[2] == '1':
        create_directory(target_dir_client)

    # 公共参数
    common_args = [
        "-I=" + protos_src_base_dir,
        "-I=/usr/include",  # 使用系统标准include路径
        f"--plugin=protoc-gen-grpc={grpc_cpp_plugin_path}"
    ]

    # 定义各组件参数
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

    # 根据组件设置执行编译
    success = True

    # 1. 编译Service组件
    if args.components[0] == '1':
        service_args = [
            f"--grpc_out={target_dir_service}",
            f"--cpp_out={target_dir_service}"
        ] + common_args + service_protos

        if not run_protoc(protoc_path, service_args, "1. Service组件"):
            success = False

    # 2. 编译Control组件
    if args.components[1] == '1':
        control_args = [
            f"--grpc_out={target_dir_control}",
            f"--cpp_out={target_dir_control}"
        ] + common_args + control_protos

        if not run_protoc(protoc_path, control_args, "2. Control组件"):
            success = False

    # 3. 编译Client组件
    if args.components[2] == '1':
        client_args = [
            f"--grpc_out={target_dir_client}",
            f"--cpp_out={target_dir_client}"
        ] + common_args + client_protos

        if not run_protoc(protoc_path, client_args, "3. Client组件"):
            success = False

    # 汇总结果
    if success:
        print("\n所有指定组件编译成功！")
        sys.exit(0)
    else:
        print("\n部分组件编译失败，请检查错误信息！")
        sys.exit(1)

if __name__ == "__main__":
    main()
