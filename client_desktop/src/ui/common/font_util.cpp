#include "common/font_util.h"
#include <QFontDatabase>
#include <QDebug>
#include <QCoreApplication>

bool FontUtil::s_fontsLoaded = false;

FontUtil& FontUtil::instance() {
    static FontUtil instance;
    return instance;
}

FontUtil::FontUtil() {
    // 确保字体已加载
    if (!s_fontsLoaded) {
        qWarning("FontUtil used before fonts loaded! Call FontUtil::loadApplicationFonts() in main()");
    }
    
    // 创建默认代码字体
    m_codeFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    if (m_codeFont.family().isEmpty()) {
        m_codeFont.setFamily("Monospace");
    }
    // m_codeFont.setPointSize(12);
}

QFont FontUtil::codeEditorFont() const {
    return m_codeFont;
}

void FontUtil::loadApplicationFonts() {
    if (s_fontsLoaded) return;
    
    const QString fontPath(":/fonts/fonts/JetBrainsMonoNL-Regular.ttf");
    const int fontId = QFontDatabase::addApplicationFont(fontPath);
    
    if (fontId != -1) {
        const QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.isEmpty()) {
            // 更新单例中的字体
            FontUtil::instance().m_codeFont.setFamily(families.first());
        } else {
            qWarning("Loaded font has no families: %s", qUtf8Printable(fontPath));
        }
    } else {
        qWarning("Failed to load application font: %s", qUtf8Printable(fontPath));
    }
    
    s_fontsLoaded = true;
}

