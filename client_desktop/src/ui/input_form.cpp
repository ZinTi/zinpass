#include "input_form.h"

InputForm::InputForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

InputForm::~InputForm(){
}

void InputForm::setup_ui(){
    edit_info_ = new QTextEdit;
    edit_info_->setHtml(
        "<h2>数据导入</h2>"
        "<p>功能尚未实现</p>"
        );
    edit_info_->setReadOnly(true);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(edit_info_);
}
