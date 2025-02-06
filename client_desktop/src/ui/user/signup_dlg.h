#ifndef SIGNUP_DLG_H
#define SIGNUP_DLG_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

class SignupDlg final : public QDialog{
    Q_OBJECT

public:
    explicit SignupDlg(QWidget *parent = nullptr);
    ~SignupDlg() override;

private slots:
    void on_btn_submit_clicked(); // 处理注册按钮点击事件的槽函数
    void on_btn_clear_clicked() const; // 处理清空按钮点击事件的槽函数

private:
    void setup_ui();

    QLabel* l_title_;
    QLabel* l_username_;
    QLabel* l_passwd1_;
    QLabel* l_passwd2_;
    QLabel* l_nickname_;
    QLineEdit* e_username_;
    QLineEdit* e_passwd1_;
    QLineEdit* e_passwd2_;
    QLineEdit* e_nickname_;

    QPushButton* btn_clear_;
    QPushButton* btn_submit_;

    QGroupBox* group_necessary_;
    QGroupBox* group_other_;
    QVBoxLayout* lyt_main_;
    QFormLayout* lyt_necessary_;
    QFormLayout* lyt_other_;
    QHBoxLayout* lyt_btn_;

};

#endif
