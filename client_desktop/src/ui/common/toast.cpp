#include "common/toast.h"
#include <QTimer>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QPainter>

Toast::Toast(QWidget* parent) : QLabel(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    
    setAlignment(Qt::AlignCenter);
    setStyleSheet("border-radius: 5px; padding: 10px;");
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Toast::fadeOut);
    
    fadeAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeAnimation->setDuration(500);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    connect(fadeAnimation, &QPropertyAnimation::finished, this, &Toast::deleteLater);
}

void Toast::showToast(QWidget* parent, const QString& text, int duration, 
                     const QColor& bgColor, const QColor& textColor) {
    Toast* toast = new Toast(parent);
    toast->setText(text);
    toast->setStyleSheet(QString(
        "border-radius: 8px; padding: 12px 20px; font: 10pt \"Segoe UI\";"
        "background-color: rgba(%1, %2, %3, %4);"
        "color: %5;"
    ).arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha())
     .arg(textColor.name()));
    
    toast->adjustSize();
    
    // 居中显示（在父窗口中心）
    if (parent) {
        QPoint center = parent->mapToGlobal(parent->rect().center());
        toast->move(center.x() - toast->width()/2, center.y() - toast->height()/2);
    } else {
        // 无父窗口则显示在屏幕中心
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        toast->move(
            (screenGeometry.width() - toast->width()) / 2,
            (screenGeometry.height() - toast->height()) * 2 / 3
        );
    }
    
    toast->show();
    toast->timer->start(duration);
}

void Toast::fadeOut() const {
    timer->stop();
    fadeAnimation->start();
}
