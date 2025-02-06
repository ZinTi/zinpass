#ifndef CHECK_PWD_SECURITY_H
#define CHECK_PWD_SECURITY_H

#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class CheckPwdSecurityForm final : public QWidget{
    Q_OBJECT

public:
    explicit CheckPwdSecurityForm(QWidget *parent = nullptr);
    ~CheckPwdSecurityForm() override;

private:
    void setup_ui();

    QTextEdit* txt_temp_; // 临时文本框

    QVBoxLayout* lyt_main_; // 主布局
};

#endif