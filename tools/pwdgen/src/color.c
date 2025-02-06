#include "color.h"

// 定义打印函数
void ColorPrintf(TextColor color, const char *format, ...) {

#ifdef _WIN32
    // Windows 平台
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD originalAttrs;

    // 获取原始控制台属性
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttrs = consoleInfo.wAttributes;

    // 定义颜色代码
    WORD fgColorMap[] = {
        0,     // 黑色
        0x4,   // 红色
        0x2,   // 绿色
        0x6,   // 黄色
        0x1,   // 蓝色
        0x5,   // 品红
        0x3,   // 青色
        0x7,   // 白色
        0x8,   // 明亮黑色
        0xC,   // 明亮红色
        0xA,   // 明亮绿色
        0xE,   // 明亮黄色
        0x9,   // 明亮蓝色
        0xD,   // 明亮品红
        0xB,   // 明亮青色
        0xF    // 明亮白色
    };

    WORD bgColorMap[] = {
        0,      // 黑色
        0x40,   // 红色
        0x20,   // 绿色
        0x60,   // 黄色
        0x10,    // 蓝色
        0x50,    // 品红
        0x30,    // 青色
        0x70,    // 白色
        0x80,    // 明亮黑色
        0xC0,    // 明亮红色
        0xA0,    // 明亮绿色
        0xE0,    // 明亮黄色
        0x90,    // 明亮蓝色
        0xD0,    // 明亮品红
        0xB0,    // 明亮青色
        0xF0     // 明亮白色
    };

    // 获取当前背景色部分
    WORD currentBg = originalAttrs & 0xF0; // 高4位是背景色
    WORD newAttrs = fgColorMap[color.fg];

    // 设置背景色
    if(color.bg != COLOR_NONE){
        newAttrs |= bgColorMap[color.bg];
    }else{
        newAttrs |= currentBg; // 保留当前背景色
    }

    // 设置颜色
    //SetConsoleTextAttribute(hConsole, fgColorMap[color.fg] | bgColorMap[color.bg]);
    SetConsoleTextAttribute(hConsole, newAttrs);

    // 打印内容
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 恢复原始属性
    SetConsoleTextAttribute(hConsole, originalAttrs);
#else
    // Linux 平台
    const char *fgColorMap[] = {
        "\033[30m", // 黑色
        "\033[31m", // 红色
        "\033[32m", // 绿色
        "\033[33m", // 黄色
        "\033[34m", // 蓝色
        "\033[35m", // 品红
        "\033[36m", // 青色
        "\033[37m", // 白色
        "\033[90m", // 明亮黑色
        "\033[91m", // 明亮红色
        "\033[92m", // 明亮绿色
        "\033[93m", // 明亮黄色
        "\033[94m", // 明亮蓝色
        "\033[95m", // 明亮品红
        "\033[96m", // 明亮青色
        "\033[97m", // 明亮白色
        "\033[0m"   // 重置
    };

    const char *bgColorMap[] = {
        "\033[40m", // 黑色
        "\033[41m", // 红色
        "\033[42m", // 绿色
        "\033[43m", // 黄色
        "\033[44m", // 蓝色
        "\033[45m", // 品红
        "\033[46m", // 青色
        "\033[47m", // 白色
        "\033[100m", // 明亮黑色
        "\033[101m", // 明亮红色
        "\033[102m", // 明亮绿色
        "\033[103m", // 明亮黄色
        "\033[104m", // 明亮蓝色
        "\033[105m", // 明亮品红
        "\033[106m", // 明亮青色
        "\033[107m", // 明亮白色
        "\033[0m"    // 重置
    };

    if(color.bg != COLOR_NONE){
        // 设置前景色和背景色
        printf("%s%s", fgColorMap[color.fg], bgColorMap[color.bg]);
    }else{
        printf("%s", fgColorMap[color.fg]);
    }

    // 打印内容
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 重置颜色
    printf("\033[0m");
#endif
}
