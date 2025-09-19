#ifndef DIALOG_EDIT_ACCOUNT_H
#define DIALOG_EDIT_ACCOUNT_H

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
#include "dlg_exposed_pwd.h"
#include "dlg_delete_account.h"

class DialogEditAccount final : public QDialog {
    Q_OBJECT

public:
    explicit DialogEditAccount(const std::string& account_id, QWidget* parent = nullptr);
    ~DialogEditAccount() override;

    QMap<QString, QVariant> get_form_data() const;
    void set_account_id(const std::string& account_id);

signals:
    void form_submitted();

private slots:
    void on_btn_read_passwd_clicked() const;
    void on_btn_delete_clicked() const;
    void on_btn_submit_clicked();

    void update_edit_main_key_state() const;

private:
    std::string account_id_;

    void setup_ui();
    void initial_input_widgets() const;
    void read_only(bool enable = true) const;

    QGroupBox* gb_platform_;
    QLineEdit* edit_provider_name_;
    QLineEdit* edit_platform_name_;
    QLineEdit* edit_url_;
    QComboBox* combo_category_;
    QLineEdit* edit_hotline_;

    QGroupBox* gb_third_party_;
    QComboBox* combo_phone_;
    QComboBox* combo_email_;

    QGroupBox* gb_account_;
    QLineEdit* edit_username_;
    QLineEdit* edit_nickname_;
    QLineEdit* edit_password_;
    QLineEdit* edit_sub_account_;

    QGroupBox* gb_other_;
    QPlainTextEdit* edit_postscript_;

    QLineEdit* edit_main_key_;

    QPushButton* btn_refresh_;
    QPushButton* btn_read_passwd_;
    QPushButton* btn_edit_;
    QPushButton* btn_submit_;
    QPushButton* btn_delete_;

    // 子对话框
    DialogDeleteAccount* dlg_delete_acc_;

    QVBoxLayout* lyt_main_;
    QGridLayout* lyt_grid_; // 创建网格布局
    QFormLayout* lyt_key_; // 创建主密钥布局
    QHBoxLayout* lyt_action_;
    QFormLayout* lyt_top_left_;
    QFormLayout* lyt_top_right_;
    QFormLayout* lyt_bottom_left_;
    QFormLayout* lyt_bottom_right_;
};

#endif
