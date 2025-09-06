#include "log_handle_form.h"

LogHandleForm::LogHandleForm(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

LogHandleForm::~LogHandleForm() {

}


void LogHandleForm::setup_ui() {
    btn_load_ = new QPushButton("加载", this);
    btn_clear_ = new QPushButton("清空", this);
    this->tb_log_ = new QTableView(this);

    lyt_main_ = new QVBoxLayout(this);
    lyt_ctl_ = new QHBoxLayout(this);

    lyt_ctl_->setAlignment(Qt::AlignRight);
    lyt_ctl_->addWidget(btn_load_);
    lyt_ctl_->addWidget(btn_clear_);
    lyt_main_->addLayout(lyt_ctl_);
    lyt_main_->addWidget(this->tb_log_);


}