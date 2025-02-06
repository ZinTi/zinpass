#ifndef ACCOUNT_FILTER_FORM_H
#define ACCOUNT_FILTER_FORM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include "popup_filter_window.h"

class AccountFilterForm final : public QWidget {
    Q_OBJECT

public:
    explicit AccountFilterForm(QWidget *parent = nullptr);
    ~AccountFilterForm() override;

    QMap<QString, QVariant> get_form_data() const;

signals:
    void form_submitted();

private slots:
    void on_btn_refresh_clicked() const;          // 处理刷新按键点击事件
    void on_btn_drop_down_more_clicked() const;   // 处理下拉按键点击事件

private:
    void setup_ui();
    void setup_layout();
    void initial_popup_window();        // 初始化下拉筛选窗口
    void initial_input_widgets() const;  // 初始化输入控件

    // 1. 基本控件
    QLineEdit* edit_provider_name_;
    QLineEdit* edit_platform_name_;
    QLineEdit* edit_username_;
    QLineEdit* edit_nickname_;
    QComboBox* combo_phone_;
    QComboBox* combo_email_;
    QComboBox* combo_category_;
    QLineEdit* edit_postscript_;
    QPushButton* btn_refresh_;
    QPushButton* btn_submit_;
    QGroupBox* box_filter_;

    // 2. 基本控件 + 自定义控件
    QToolButton* btn_drop_down_more_;    // 下拉按钮
    PopupFilterWindow* popup_window_;    // 下拉弹窗

    // 3. 布局
    QHBoxLayout* lyt_main_;          // 主布局
    QHBoxLayout* lyt_gbox_;          // 筛选框布局
    QFormLayout* lyt_form_left_;     // 左侧表单布局
    QFormLayout* lyt_form_right_;    // 右侧表单布局
    QVBoxLayout* lyt_btn_;           // 按钮布局

};

#endif // ACCOUNT_FILTER_FORM_H
