#include "account/data_owner_form.h"

DataOwnerForm::DataOwnerForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

DataOwnerForm::~DataOwnerForm() = default;


void DataOwnerForm::setup_ui() {
    this->tv_data_ = new QTableView(this);
    this->tv_data_->setMinimumSize(1000, 500);

    this->lbl_tips_= new QLabel("需要输入另一个数据所有者的账号用户名和主密钥才能完成数据迁移。", this);
    this->lbl_your_key_= new QLabel("你的主密钥", this);
    this->edit_your_key_= new QLineEdit(this);
    this->lbl_another_username_= new QLabel("目标用户名", this);
    this->edit_another_username_= new QLineEdit(this);
    this->lbl_another_key_ = new QLabel("目标主密钥", this);
    this->edit_another_key_= new QLineEdit(this);

    this->lyt_form_ = new QFormLayout(this);
    this->lyt_form_->addRow(this->lbl_your_key_, this->edit_your_key_);
    this->lyt_form_->addRow(this->lbl_another_username_, this->edit_another_username_);
    this->lyt_form_->addRow(this->lbl_another_key_, this->edit_another_key_);

    this->lyt_center_ = new QHBoxLayout(this);
    this->lyt_center_->addWidget(this->tv_data_);
    this->lyt_center_->addLayout(this->lyt_form_);

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_main_->addWidget(this->lbl_tips_);
    this->lyt_main_->addLayout(this->lyt_center_);

    this->setLayout(this->lyt_main_);
}

