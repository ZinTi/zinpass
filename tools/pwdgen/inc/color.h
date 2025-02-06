#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef INC_COLOR_H
#define INC_COLOR_H

#ifdef _WIN32
    #include <windows.h>
#endif

// 定义颜色枚举
typedef enum {
    COLOR_NONE = -1, // 不设置颜色
    COLOR_BLACK = 0, // 黑色
    COLOR_RED,       // 红色
    COLOR_GREEN,     // ...
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_BRIGHT_BLACK,
    COLOR_BRIGHT_RED,
    COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_YELLOW,
    COLOR_BRIGHT_BLUE,
    COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_CYAN,
    COLOR_BRIGHT_WHITE,
    COLOR_RESET // 默认颜色（重置）
} ANSI_Color;

typedef struct TextColor{
   ANSI_Color fg; // 前景色
   ANSI_Color bg; // 背景色
}TextColor;

/**
 * @brief 彩色格式化打印函数
 * @param color 结构体类型，包括两个枚举类型——前景色和背景色
 * @param format 字符串格式
 * @param ... 其他变量
 */
void ColorPrintf(TextColor color, const char *format, ...);

#endif // INC_COLOR_H