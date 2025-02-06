import os
import subprocess

# 获取当前脚本所在路径的父路径
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(CURRENT_DIR)
print("Project root: " + PROJECT_ROOT)

# 设置 gRPC 安装路径
GRPC_INSTALL_DIR = "A:/build/grpc-1.71.0-amd64-win-mingw64"

protos_src_base_dir = PROJECT_ROOT + "/protos"
target_dir_service = PROJECT_ROOT + "/service/auto_generated"                 # 后台服务/守护进程目标目录
target_dir_control = PROJECT_ROOT + "/control/generate"                 # 管理控制客户端目标目录
target_dir_client = PROJECT_ROOT + "/client_desktop/generate/grpc"      # 桌面客户端目标目录
# 若目标目录不存在则创建（自动处理多级目录）
try:
    os.makedirs(target_dir_service, exist_ok=True)
except OSError as e:
    print(f"创建 service 目标目录失败：{e}")

try:
    os.makedirs(target_dir_control, exist_ok=True)
except OSError as e:
    print(f"创建 control 目标目录失败：{e}")

try:
    os.makedirs(target_dir_client, exist_ok=True)
except OSError as e:
    print(f"创建 client_desktop 目标目录失败：{e}")

# protoc path
protoc_path = GRPC_INSTALL_DIR + "/bin/protoc.exe"
grpc_cpp_plugin_path = GRPC_INSTALL_DIR + "/bin/grpc_cpp_plugin.exe"

arguments_service = ["--grpc_out=" + target_dir_service,
                    "--cpp_out=" + target_dir_service,
                    "-I=" + protos_src_base_dir,
                    "-I=" + GRPC_INSTALL_DIR + "/include/google/protobuf/",
                    "--plugin=protoc-gen-grpc=" + grpc_cpp_plugin_path,
                    protos_src_base_dir + "/common/v1/common.proto",                    
                    protos_src_base_dir + "/hello/hello.proto",
                    protos_src_base_dir + "/control/status.proto",

                    protos_src_base_dir + "/user/v1/user.proto",
                    protos_src_base_dir + "/user/v1/auth.proto",
                    protos_src_base_dir + "/account/v1/phone.proto",
                    protos_src_base_dir + "/account/v1/account.proto",
                    protos_src_base_dir + "/account/v1/category.proto",
                    protos_src_base_dir + "/account/v1/telecom.proto"
                    ]

arguments_control = ["--grpc_out=" + target_dir_control,
                    "--cpp_out=" + target_dir_control,
                    "-I=" + protos_src_base_dir,
                    "-I=" + GRPC_INSTALL_DIR + "/include/google/protobuf/",
                    "--plugin=protoc-gen-grpc=" + grpc_cpp_plugin_path,
                    protos_src_base_dir + "/common/v1/common.proto",                    
                    protos_src_base_dir + "/hello/hello.proto",
                    protos_src_base_dir + "/control/status.proto"
                    ]

arguments_client = ["--grpc_out=" + target_dir_client,
                    "--cpp_out=" + target_dir_client,
                    "-I=" + protos_src_base_dir,
                    "-I=" + GRPC_INSTALL_DIR + "/include/google/protobuf/",
                    "--plugin=protoc-gen-grpc=" + grpc_cpp_plugin_path,
                    protos_src_base_dir + "/common/v1/common.proto",
                    protos_src_base_dir + "/user/v1/user.proto",
                    protos_src_base_dir + "/user/v1/auth.proto",
                    protos_src_base_dir + "/account/v1/phone.proto",
                    protos_src_base_dir + "/account/v1/account.proto",
                    protos_src_base_dir + "/account/v1/category.proto",
                    protos_src_base_dir + "/account/v1/telecom.proto"
                    ]

# 1. service 执行命令并捕获结果
result_service = subprocess.run(
    [protoc_path] + arguments_service,
    capture_output=True,  # 捕获输出内容
    text=True,            # 以文本形式返回输出（Python 3.7+）
    check=True            # 若返回码非零，抛出异常
)

# 输出结果
print("=== 1. ZinPassSvc(/zinpassd) ===")
print("返回码:", result_service.returncode)
print("标准输出:", result_service.stdout)
print("错误输出:", result_service.stderr)
print("Proto files (Server) generated to " + target_dir_service)

# 2. control 执行命令并捕获结果
result_control = subprocess.run(
    [protoc_path] + arguments_control,
    capture_output=True,  # 捕获输出内容
    text=True,            # 以文本形式返回输出（Python 3.7+）
    check=True            # 若返回码非零，抛出异常
)

# 输出结果
print("=== 2. zinpassctl ===")
print("返回码:", result_control.returncode)
print("标准输出:", result_control.stdout)
print("错误输出:", result_control.stderr)
print("Proto files (Server) generated to " + target_dir_control)

# 3. client_desktop 执行命令并捕获结果
result_desktop = subprocess.run(
    [protoc_path] + arguments_client,
    capture_output=True,  # 捕获输出内容
    text=True,            # 以文本形式返回输出（Python 3.7+）
    check=True            # 若返回码非零，抛出异常
)

# 输出结果
print("=== 3. zinpass-client ===")
print("返回码:", result_desktop.returncode)
print("标准输出:", result_desktop.stdout)
print("错误输出:", result_desktop.stderr)
print("Proto files (Client) generated to " + target_dir_client)

