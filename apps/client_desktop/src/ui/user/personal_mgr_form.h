#ifndef PERSONAL_MGR_FORM_H
#define PERSONAL_MGR_FORM_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFormLayout>

class PersonalMgrForm final : public QWidget {
    Q_OBJECT

public:
    explicit PersonalMgrForm(QWidget *parent = nullptr);
    ~PersonalMgrForm() override;

private slots:
    void on_btn_refresh_clicked();  // refresh the personal information
    void on_btn_save_clicked();     // save the personal information
    void on_btn_ch_pwd_clicked();   // change password
    void on_btn_cancel_acc_clicked();  // cancel account

private:
    void setup_ui();
    void get_personal_info();

    QLabel* l_title_;
    QLabel* l_id_;
    QLabel* l_username_;
    QLabel* l_nickname_;
    QLabel* l_created_at_;
    QLabel* l_updated_at_;
    QLabel* l_old_password_;
    QLabel* l_new_password_;
    QLabel* l_confirm_password_;

    QLineEdit* e_id_;
    QLineEdit* e_username_;
    QLineEdit* e_nickname_;
    QLineEdit* e_created_at_;
    QLineEdit* e_updated_at_;
    QLineEdit* e_old_password_;
    QLineEdit* e_new_password_;
    QLineEdit* e_confirm_password_;

    QPushButton* btn_refresh_;
    QPushButton* btn_save_;
    QPushButton* btn_ch_pwd_;
    QPushButton* btn_cancel_acc_;

    QTextEdit* e_info_;

    QGroupBox* box_info_;
    QGroupBox* box_pwd_;

    std::string user_id_;
    std::string username_;
    std::string nickname_;
    std::string created_at_;
    std::string updated_at_;

    // layout
    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_bottom_;
    QFormLayout* lyt_base_info_;
    QFormLayout* lyt_pwd_;
    QVBoxLayout* lyt_btn_;

};

#endif
