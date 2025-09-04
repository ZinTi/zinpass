#ifndef DIALOG_ADD_PHONE_H
#define DIALOG_ADD_PHONE_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

class DialogAddPhone final : public QDialog {
    Q_OBJECT

public:
    explicit DialogAddPhone(QWidget* parent = nullptr);
    ~DialogAddPhone() override;

private slots:
    void on_btn_refresh_clicked() const;
    void on_btn_clear_clicked() const;
    void on_btn_submit_clicked();

private:
    // 控件
    QLineEdit* edit_phone_;
    QComboBox* combo_tele_operator_;
    QLineEdit* edit_service_password_;
    QLineEdit* edit_pin_;
    QLineEdit* edit_puk_;
    QDateTimeEdit* edit_join_time_;
    QLineEdit* edit_phone_area_;
    QTextEdit* edit_postscript_;
    QLineEdit* edit_main_key_;

    QPushButton* btn_refresh_;
    QPushButton* btn_clear_;
    QPushButton* btn_submit_;

    // 布局
    QVBoxLayout* lyt_main_;
    QFormLayout* lyt_form_;
    QHBoxLayout* lyt_btn_;

    void setup_ui();
    void initial_edit_widget() const;

};

#endif // DIALOG_ADD_PHONE_H
