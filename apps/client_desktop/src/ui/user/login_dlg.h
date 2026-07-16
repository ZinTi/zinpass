#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QPixmap>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include "settings_dlg.h"

class LoginDlg final : public QDialog {
    Q_OBJECT

public:
    LoginDlg(QWidget* parent = nullptr);
    ~LoginDlg() override;

private slots:  // 槽函数
    void on_btn_settings_clicked() const; // 处理设置按钮点击事件
    void on_btn_login_clicked(); // 处理登录按钮点击事件
    void on_btn_signup_clicked(); // 处理注册按钮点击事件

private:
    SettingsDlg* settings_dlg_; // 登录对话框的设置对话框

    QToolButton* btn_settings_; // 登录对话框右上角的设置按钮
    QLabel* l_title_;
    QLabel* l_logo_;
    QPixmap* pixmap_logo_;

    QLabel* l_username_;
    QLabel* l_key_;

    QComboBox* combo_username_;
    QLineEdit* e_key_;

    QPushButton* btn_login_;
    QPushButton* btn_signup_;

    QLabel* l_copyleft_;

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_top_;
    QFormLayout* lyt_form_;
    QHBoxLayout* lyt_btn_;

    void setup_ui();
    void refresh_combo_username() const;
};

#endif
