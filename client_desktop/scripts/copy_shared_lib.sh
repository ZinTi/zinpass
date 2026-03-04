#!/bin/env bash

# 检查zinpass文件是否存在
if [ ! -f "zinpass" ]; then
    echo "错误: 当前目录下没有找到zinpass文件"
    exit 1
fi

# 使用ldd获取依赖库并复制到当前目录
echo "正在检查zinpass的依赖库..."
ldd "zinpass" 2>/dev/null | while IFS= read -r line; do
    if [[ $line == *"=>"* ]]; then
        # 提取库文件路径
        libpath=$(echo "$line" | awk '{print $3}')
        if [ -n "$libpath" ] && [ -f "$libpath" ]; then
            echo "复制: $libpath"
            cp "$libpath" .
        else
            echo "警告: 跳过无效库路径: $libpath"
        fi
    fi
done

echo "依赖库复制完成。"
