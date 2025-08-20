#include "account/check_pwd_security_form.h"

#include <QVBoxLayout>

CheckPwdSecurityForm::CheckPwdSecurityForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

CheckPwdSecurityForm::~CheckPwdSecurityForm(){
}

void CheckPwdSecurityForm::setup_ui() {
    this->txt_temp_ = new QTextEdit(this);
    this->txt_temp_->setReadOnly(true);
    this->txt_temp_->setHtml(
        "<h3>安全检查</h3>"
        "<p>此功能尚未实现。开发目标：</p>"
        "<p>1、检查密码重复使用、弱密码、长久未修改密码等，提供密码风险提醒；</p>"
        "<p>2、检查手机号、邮箱账号风险，提供提醒；</p>"
    );

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->setSpacing(5);
    lyt_main_->setContentsMargins(12, 12, 12, 12);
    lyt_main_->addWidget(txt_temp_);
    this->setLayout(lyt_main_);
}
