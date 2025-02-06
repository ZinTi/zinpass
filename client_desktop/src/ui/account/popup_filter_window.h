#ifndef POPUP_FILTER_WINDOW_H
#define POPUP_FILTER_WINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QEvent>
#include <QMouseEvent>


// 自定义弹出窗口类
class PopupFilterWindow final : public QWidget{
    Q_OBJECT
public:
    explicit PopupFilterWindow(QWidget *parent = nullptr);
    ~PopupFilterWindow() override;

protected:
    bool event(QEvent *event) override; // 重写鼠标按下事件，处理窗口外部点击

private:
    QLabel* l_title_; // 标题
    QPushButton* btn_reset_; // 清空（重置）按钮

    QLabel* l_url_;
    QLineEdit* e_url_;
    QLabel* l_hotline_;
    QLineEdit* e_hotline_;
    QLabel* l_sub_acc_;
    QLineEdit* e_sub_acc_;
    QLabel* l_cdt_range_;
    QDateTimeEdit* e_cdt_range_start_;
    QDateTimeEdit* e_cdt_range_end_;
    QLabel* l_udt_range_;
    QDateTimeEdit* e_udt_range_start_;
    QDateTimeEdit* e_udt_range_end_;

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_btn_;
    QVBoxLayout* lyt_cdt_range_;
    QVBoxLayout* lyt_udt_range_;
    QFormLayout* lyt_filter_;
};
#endif
