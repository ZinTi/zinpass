#include "output_form.h"

OutputForm::OutputForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

OutputForm::~OutputForm(){
}

void OutputForm::setup_ui(){
    edit_info_ = new QTextEdit(this);
    edit_info_->setHtml(
        "<h2>数据导出</h2>"
        "<p>功能尚未实现</p>"
        );
    edit_info_->setReadOnly(true);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(edit_info_);
}
