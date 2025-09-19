#!/bin/bash
# 跨发行版兼容的依赖复制脚本（完整文件复制版）

if [ $# -ne 1 ]; then
    echo "用法: $0 <可执行文件>"
    exit 1
fi

if ! command -v patchelf &> /dev/null; then
    echo "错误: patchelf 未安装"
    echo "Debian/Ubuntu: sudo apt install patchelf"
    echo "Fedora/RHEL: sudo dnf install patchelf"
    echo "Alpine: sudo apk add patchelf"
    exit 1
fi

BIN="$1"
DEST_DIR="$(dirname "$BIN")/libs"
mkdir -p "$DEST_DIR"

# 创建临时工作目录
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

echo "正在收集依赖库..."
ldd "$BIN" 2>/dev/null | awk '
  /=> \// { print $3 }
  /^\// && !/=>/ { print }
  /musl/ { print $1 }
' | grep -Ev "ld-linux|linux-vdso|ld-musl" | sort -u | while read -r lib; do
    if [ -f "$lib" ]; then
        # 获取实际文件路径（解引用所有软链接）
        real_path=$(readlink -f "$lib")
        
        # 复制实际文件到临时目录（保持原始文件名）
        cp -L -v --preserve=all "$real_path" "$TEMP_DIR/$(basename "$real_path")"
        
        # 如果原始路径是软链接，记录链接关系
        if [ -L "$lib" ]; then
            link_name=$(basename "$lib")
            target_name=$(basename "$real_path")
            echo "$link_name $target_name" >> "$TEMP_DIR/link_mappings.txt"
        fi
    else
        echo "警告: 跳过不存在的库: $lib"
    fi
done

# 复制所有实际文件到目标目录
echo "复制库文件到目标目录..."
cp -v "$TEMP_DIR"/*.so* "$DEST_DIR/" 2>/dev/null

# 创建软链接
if [ -f "$TEMP_DIR/link_mappings.txt" ]; then
    echo "创建必要的软链接..."
    while read -r link_name target_name; do
        # 确保目标文件存在
        if [ -f "$DEST_DIR/$target_name" ]; then
            ln -sf "$target_name" "$DEST_DIR/$link_name"
            echo "创建链接: $link_name -> $target_name"
        else
            echo "警告: 目标文件不存在: $target_name"
        fi
    done < "$TEMP_DIR/link_mappings.txt"
fi

# 添加执行权限（仅文件）
find "$DEST_DIR" -type f -exec chmod +x {} +

# 增强版动态链接器查找
INTERPRETER=""
SEARCH_PATHS=("/lib" "/lib64" "/usr/lib" "/usr/lib64" "/usr/local/lib")

# 尝试查找glibc链接器
for path in "${SEARCH_PATHS[@]}"; do
    [ -d "$path" ] || continue
    candidate=$(find "$path" -maxdepth 1 -name 'ld-linux*.so*' -print -quit 2>/dev/null)
    [ -n "$candidate" ] && { INTERPRETER="$candidate"; break; }
done

# 尝试查找musl链接器
if [ -z "$INTERPRETER" ]; then
    musl_path=$(ldd "$BIN" 2>/dev/null | awk '/musl/ {print $3; exit}' | xargs dirname 2>/dev/null)
    [ -n "$musl_path" ] && INTERPRETER=$(find "$musl_path" -name 'ld-musl*.so*' -print -quit)
fi

# 最终回退方案
if [ -z "$INTERPRETER" ]; then
    echo "警告: 自动查找动态链接器失败，尝试备用方法"
    INTERPRETER=$(find / -path '*/ld-*.so*' -not -path '/sys/*' -print -quit 2>/dev/null)
fi

# 交互式回退
if [ -z "$INTERPRETER" ]; then
    echo "⚠️ 严重警告: 未找到动态链接器"
    read -p "是否手动指定路径? (y/N) " choice
    if [[ "$choice" =~ ^[Yy] ]]; then
        read -p "输入完整路径: " INTERPRETER
        [ ! -f "$INTERPRETER" ] && echo "无效路径，跳过修改" && INTERPRETER=""
    fi
fi

if [ -n "$INTERPRETER" ]; then
    patchelf --set-interpreter "$INTERPRETER" "$BIN"
    echo "动态链接器修改为: $INTERPRETER"
else
    echo "警告: 未设置动态链接器，程序可能无法跨平台运行"
    echo "提示: 可尝试在目标系统上手动执行:"
    echo "      patchelf --set-interpreter \$(find /lib* -name 'ld-*.so*' | head -1) '$BIN'"
fi

# 设置RPATH
patchelf --set-rpath '$ORIGIN/libs' "$BIN"

echo "依赖库已复制到: $DEST_DIR"
echo "RPATH 设置为: \$ORIGIN/libs"
echo "完成! 请将整个目录($(dirname "$BIN"))复制到目标系统"

# 验证复制结果
echo -e "\n验证库文件:"
find "$DEST_DIR" -type f -exec file {} \; | grep -E 'ELF|shared object'
