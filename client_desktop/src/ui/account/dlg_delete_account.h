#ifndef DIALOG_DELETE_ACCOUNT_H
#define DIALOG_DELETE_ACCOUNT_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <string>

class DialogDeleteAccount final : public QDialog {
    Q_OBJECT

public:
    explicit DialogDeleteAccount(const std::string& account_id, QWidget* parent = nullptr);
    ~DialogDeleteAccount() override;

    void set_account_id(const std::string& account_id);
    std::string get_input_main_key() const;

private slots:
    void on_btn_okay_clicked();

private:
    QLineEdit* edit_password_;
    QPushButton* btn_okay_;
    QPushButton* btn_cancel_;

    std::string account_id_;
    std::string main_key_;

    void setup_ui();
};

#endif // DIALOG_DELETE_ACCOUNT_H
