# Directory Layout │ 推荐的目录布局（结构）

此文档介绍本项目推荐的安装部署目录结构

## Contents 目录


## 一、Release Package Naming Format - 发版包命名格式说明  

- **Format:** 
- <Name>-<Component>-<Version>-<Platform>-<Architecture>-<Toolchain>.<Extension>
- 
- **格式：** <名称>-<组件>-<版本>-<平台>-<架构>-<编译器>.<扩展名>  
    - 名称：zinpass
    - 组件：server（服务端）或 client（桌面客户端）
    - 版本：如 v1.0.0
    - 平台：linux, windows
    - 架构：x86_64, aarch64
    - 扩展名：.tar.xz , .7z
  
```plaintext
# 例如

1. 服务端
zinpass-server-v1.0.0-linux-x86_64-gcc.tar.xz           √
zinpass-server-v1.0.0-linux-x86_64-clang.tar.xz         -
zinpass-server-v1.0.0-linux-aarch64-gcc.tar.xz          ×
zinpass-server-v1.0.0-linux-aarch64-clang.tar.xz        ×

zinpass-server-v1.0.0-windows-x86_64-mingw64.7z         √
zinpass-server-v1.0.0-windows-x86_64-msvc.7z            -
zinpass-server-v1.0.0-windows-aarch64-mingw64.7z        ×
zinpass-server-v1.0.0-windows-aarch64-msvc.7z           ×

2. 桌面客户端
zinpass-client-v1.0.0-linux-x86_64-gcc.tar.xz           √
zinpass-client-v1.0.0-linux-x86_64-clang.tar.xz         -

zinpass-client-v1.0.0-windows-x86_64-mingw64.7z         √
zinpass-client-v1.0.0-windows-x86_64-msvc.7z            -

* √ 表示提供预构建包，- 表示未来可能提供预构建包，× 表示没有计划提供适配
```

## 一、部署服务端

GNU/Linux 部署
```plaintext
/opt/zinpass-server/
├─ bin/
│   ├─ zinpassd           # Deamon
│   ├─ zinpassctl         # CLI Admin Tool
│   ├─ service.config     # Configuration
│   └─ ...
│
└─ data/
    ├─ service.log        # Log file
    ├─ zinpass.db         # Database file
    └─ ...
```

Windows 部署
```plaintext
D:\app\zinpass-server
├─ bin
│   ├─ zinpasssvc.exe     # Service
│   ├─ zinpassctl.exe     # CLI Admin Tool
│   ├─ service.config     # Configuration
│   └─ ...
│
└─ data
    ├─ service.log        # Log file
    └─ zinpass.db         # Database file
```


## 二、部署客户端

GNU/Linux 部署
```plaintext
/opt/zinpass-client/
└─ bin/
    ├─ zinpass             # Client
    ├─ client.config       # Configuration
    ├─ client.log          # Log file
    ├─ pwdgen              # PasswdGenerator
    └─ libbinhub.so        # Dy Lib
```

Windows 部署
```plaintext
D:\app\zinpass-client
└─ bin
    ├─ zinpass.exe
    ├─ client.config
    ├─ client.log
    ├─ pwdgen.exe
    └─ libbinhub.dll
```

## 三、将客户端和服务端部署在同一平台

若将服务端和客户端部署在同一平台上，推荐以下目录布局。

GNU/Linux 部署
```plaintext
/opt/zinpass-server/
├─ bin/
│   ├─ zinpass            # Client
│   ├─ zinpassd           # Deamon
│   ├─ zinpassctl         # CLI Admin Tool
│   ├─ client.config      # Configuration
│   ├─ service.config     # Configuration
│   └─ ...
│
└─ data/
    ├─ client.log         # Log file
    ├─ service.log        # Log file
    ├─ zinpass.db         # Database file
    ├─ pwdgen             # PasswdGenerator
    └─ libbinhub.so       # Dy Lib

```


Windows 部署
```plaintext
D:\app\zinpass-server
├─ bin
│   ├─ zinpass.exe
│   ├─ zinpasssvc.exe     # Service
│   ├─ zinpassctl.exe     # CLI Admin Tool
│   ├─ client.config      # Configuration
│   ├─ service.config     # Configuration
│   ├─ pwdgen.exe
│   └─ libbinhub.dll
│
└─ data
    ├─ client.log
    ├─ service.log        # Log file
    └─ zinpass.db         # Database file
```

将服务端和客户端部署在同一平台请自行解决数据维护。