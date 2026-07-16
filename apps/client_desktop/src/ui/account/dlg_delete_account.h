#ifndef DIALOG_DELETE_ACCOUNT_H
#define DIALOG_DELETE_ACCOUNT_H

#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>

class DialogDeleteAccount final : public QDialog {
    Q_OBJECT

public:
    explicit DialogDeleteAccount(const std::string& account_id, QWidget* parent = nullptr);
    ~DialogDeleteAccount() override;

    void set_account_id(const std::string& account_id);
    std::string get_input_main_key() const;

private slots:
    void on_btn_confirm_clicked();

signals:
    void remove_result(bool success);

private:
    QLabel* lbl_main_key_;
    QLineEdit* edit_main_key_;
    QPushButton* btn_confirm_;
    QPushButton* btn_cancel_;

    std::string account_id_;
    std::string main_key_;

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_pwd_;
    QHBoxLayout* lyt_btn_;

    void setup_ui();
};

#endif // DIALOG_DELETE_ACCOUNT_H
