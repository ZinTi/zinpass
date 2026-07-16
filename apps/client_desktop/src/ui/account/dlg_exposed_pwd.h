#ifndef DIALOG_EXPOSED_PWD_H
#define DIALOG_EXPOSED_PWD_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

class DialogExposedPwd final : public QDialog{
    Q_OBJECT

public:
    explicit DialogExposedPwd(const std::string& account_id, QWidget *parent = nullptr);
    ~DialogExposedPwd() override;

private slots:
    void start_btn_countdown(QPushButton* btn);
    void update_button();

    void on_btn_read_clicked();             // 查看
    void on_btn_copy_clicked() const;             // 仅复制
    void on_btn_read_and_copy_clicked();    // 查看并复制

private:
    std::string account_id_;

    void setup_ui();

    QTimer* timer_;     // 定时器
    int countdown_ = 0; // 倒计时秒数
    QString original_btn_text_;
    QPushButton* current_btn_;

    QTextEdit* edit_display_;
    QLineEdit* edit_main_key_;
    QPushButton* btn_read_;
    QPushButton* btn_copy_;
    QPushButton* btn_read_and_copy_;

    QVBoxLayout* lyt_main_;
    QFormLayout* lyt_main_key_;
    QHBoxLayout* lyt_btn_;

};

#endif
