#include "input_form.h"

InputForm::InputForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

InputForm::~InputForm(){
}

void InputForm::setup_ui(){
    this->edit_info_ = new QTextEdit;
    this->edit_info_->setHtml(
        "<h2>数据导入</h2>"
        "<p>功能尚未实现</p>"
        );
    this->edit_info_->setReadOnly(true);

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_main_->addWidget(this->edit_info_);
}
