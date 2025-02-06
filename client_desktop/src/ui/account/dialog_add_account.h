#ifndef DIALOG_ADD_ACCOUNT_H
#define DIALOG_ADD_ACCOUNT_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QPushButton>

class DialogAddAccount final : public QDialog {
    Q_OBJECT

public:
    explicit DialogAddAccount(QWidget *parent = nullptr);
    ~DialogAddAccount() override;

private slots:
    void on_btn_refresh_clicked() const;  // 刷新
    void on_btn_clear_clicked() const;    // 清空
    void on_btn_submit_clicked();   // 提交

private:
    void setup_ui();
    void initial_input_widgets() const;

    QGroupBox* group_box_platform_;
    QLineEdit* edit_provider_name_;
    QLineEdit* edit_platform_name_;
    QLineEdit* edit_url_;
    QComboBox* combo_category_;
    QLineEdit* edit_hotline_;

    QGroupBox* group_box_third_party_;
    QComboBox* combo_phone_;
    QComboBox* combo_email_;

    QGroupBox* group_box_account_;
    QLineEdit* edit_username_;
    QLineEdit* edit_nickname_;
    QLineEdit* edit_password_;
    QLineEdit* edit_sub_account_;

    QGroupBox* group_box_other_;
    QPlainTextEdit* edit_postscript_;

    QLineEdit* edit_main_key_;     // 主密钥

    QPushButton* btn_refresh_;  // 刷新
    QPushButton* btn_clear_;    // 清空输入
    QPushButton* btn_submit_;

    QVBoxLayout* lyt_main_;
    QGridLayout* lyt_grid_;
    QFormLayout* lyt_form_key_;
    QHBoxLayout* lyt_key_and_btn_;
    QFormLayout* lyt_top_left_;
    QFormLayout* lyt_top_right_;
    QFormLayout* lyt_bottom_left_;
    QFormLayout* lyt_bottom_right_;
};

#endif // DIALOG_ADD_ACCOUNT_H
