#include "log_run_form.h"


LogRunForm::LogRunForm(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

LogRunForm::~LogRunForm() {
}

void LogRunForm::setup_ui() {
    e_main_key_ = new QLineEdit(this);
    e_main_key_->setEchoMode(QLineEdit::Password);
    e_main_key_->setPlaceholderText("Enter main key, please...");

    btn_load_ = new QPushButton("Fetch", this);
    btn_save_ = new QPushButton("Download", this);
    e_app_log_ = new QTextEdit(this);
    e_app_log_->setReadOnly(true);
    e_app_log_->setPlaceholderText("客户端本地运行日志(非服务器运行日志)");

    lyt_main_ = new QVBoxLayout(this);
    lyt_ctl_ = new QHBoxLayout(this);

    lyt_ctl_->addWidget(e_main_key_);
    lyt_ctl_->addWidget(btn_load_);
    lyt_ctl_->addWidget(btn_save_);

    lyt_main_->addLayout(lyt_ctl_);
    lyt_main_->addWidget(e_app_log_);

}