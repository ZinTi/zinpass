#!/bin/env bash

# 获取当前系统类型
SYSTEM=$(uname -s)

# 遍历 install 目录下的所有直接子目录
for dir in ./install/*/; do
    # 去除路径末尾的斜杠，获取目录名
    dirname=$(basename "$dir")
    
    # 创建压缩包，保留目录结构
    case "$SYSTEM" in
        Linux*)
            echo "OS: Linux"
            tar -cJf "${dirname}.tar.xz" -C ./install "${dirname}"
            ;;
        Darwin*)
            echo "OS: macOS"
            tar -cJf "${dirname}.tar.xz" -C ./install "${dirname}"
            ;;
        CYGWIN*|MINGW*|MSYS*)
            echo "OS: Windows (Git Bash/Cygwin/MSYS)"
            7z a -r "${dirname}.7z" "./install/${dirname}"
            ;;
        *)
            echo "OS: $SYSTEM"
            7z a -r "${dirname}.7z" "./install/${dirname}"
            ;;
    esac
    
    echo "Packaged: ${dirname}.tar.xz"
done

echo "Packaging complete!"
