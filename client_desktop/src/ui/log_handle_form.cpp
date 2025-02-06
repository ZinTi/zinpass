#include "log_handle_form.h"

LogHandleForm::LogHandleForm(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

LogHandleForm::~LogHandleForm() {

}


void LogHandleForm::setup_ui() {
    this->btn_load_ = new QPushButton("加载", this);
    this->btn_clear_ = new QPushButton("清空", this);
    this->tb_log_ = new QTableView(this);

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_ctl_ = new QHBoxLayout(this);

    this->lyt_ctl_->setAlignment(Qt::AlignRight);
    this->lyt_ctl_->addWidget(this->btn_load_);
    this->lyt_ctl_->addWidget(this->btn_clear_);
    this->lyt_main_->addLayout(this->lyt_ctl_);
    this->lyt_main_->addWidget(this->tb_log_);


}