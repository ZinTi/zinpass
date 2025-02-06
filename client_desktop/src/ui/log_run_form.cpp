#include "log_run_form.h"


LogRunForm::LogRunForm(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

LogRunForm::~LogRunForm() {
}

void LogRunForm::setup_ui() {
    this->e_main_key_ = new QLineEdit(this);
    this->e_main_key_->setEchoMode(QLineEdit::Password);
    this->e_main_key_->setPlaceholderText("Enter main key, please...");

    this->btn_load_ = new QPushButton("Fetch", this);
    this->btn_save_ = new QPushButton("Download", this);
    this->e_app_log_ = new QTextEdit(this);
    this->e_app_log_->setReadOnly(true);
    this->e_app_log_->setPlaceholderText("客户端本地运行日志(非服务器运行日志)");

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_ctl_ = new QHBoxLayout(this);

    this->lyt_ctl_->addWidget(this->e_main_key_);
    this->lyt_ctl_->addWidget(this->btn_load_);
    this->lyt_ctl_->addWidget(this->btn_save_);

    this->lyt_main_->addLayout(this->lyt_ctl_);
    this->lyt_main_->addWidget(this->e_app_log_);

}