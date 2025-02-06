#ifndef FONT_UTIL_H
#define FONT_UTIL_H

#include <QFont>
#include <QString>

class FontUtil {
public:
    // 获取单例实例
    static FontUtil& instance();
    
    // 禁止复制和移动
    FontUtil(const FontUtil&) = delete;
    FontUtil& operator=(const FontUtil&) = delete;
    
    // 获取代码编辑器字体
    QFont codeEditorFont() const;
    
    // 预加载应用程序字体（在main函数中调用）
    static void loadApplicationFonts();

private:
    // 私有构造函数
    FontUtil();
    
    QFont m_codeFont;
    static bool s_fontsLoaded;
};

#endif
