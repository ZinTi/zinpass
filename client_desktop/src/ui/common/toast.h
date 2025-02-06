// 仿安卓 toast 控件
#ifndef TOAST_H
#define TOAST_H

#include <QLabel>
#include <QPropertyAnimation>

class Toast : public QLabel {
    Q_OBJECT
public:
    explicit Toast(QWidget* parent = nullptr);
    
    static void showToast(QWidget* parent, const QString& text, 
                         int duration = 3000,
                         const QColor& bgColor = QColor(50, 50, 50, 200),
                         const QColor& textColor = Qt::white);
    
private:
    void fadeOut() const;
    QTimer* timer;
    QPropertyAnimation* fadeAnimation;
};
#endif
