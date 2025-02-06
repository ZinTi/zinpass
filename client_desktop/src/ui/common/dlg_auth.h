#ifndef DIALOG_AUTH_H
#define DIALOG_AUTH_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class DialogAuth final : public QDialog {
public:
    explicit DialogAuth(QWidget* parent = nullptr);
    ~DialogAuth() override;

    QString get_input_main_key() const;

private slots:
    void on_btn_okay_clicked();

private:
    QLabel* l_password_;

    QLineEdit* edit_password_;
    QPushButton* btn_okay_;
    QPushButton* btn_cancel_;

    QString main_key_;

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_pwd_;
    QHBoxLayout* lyt_btn_;

};

#endif // DIALOG_AUTH_H
