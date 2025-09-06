#include "common/dlg_auth.h"
#include <QApplication>

#include <QMessageBox>
// #include "authentication.h"

DialogAuth::DialogAuth(QWidget* parent) : QDialog(parent) {
    setWindowTitle("身份验证");
    setFixedSize(200, 120);

    // 垂直布局管理器
    lyt_main_ = new QVBoxLayout(this);

    // 密码标签和输入框
    lyt_pwd_ = new QHBoxLayout(this);
    l_password_ = new QLabel("主密钥", this);
    edit_password_ = new QLineEdit(this);
    edit_password_->setEchoMode(QLineEdit::Password);
    lyt_pwd_->addWidget(l_password_);
    lyt_pwd_->addWidget(edit_password_);
    lyt_main_->addLayout(lyt_pwd_);

    // 按钮布局
    lyt_btn_ = new QHBoxLayout(this);
    btn_okay_ = new QPushButton("确定", this);
    btn_cancel_ = new QPushButton("取消", this);
    lyt_btn_->addWidget(btn_okay_);
    lyt_btn_->addWidget(btn_cancel_);
    lyt_main_->addLayout(lyt_btn_);

    connect(btn_okay_, &QPushButton::clicked, this, &DialogAuth::on_btn_okay_clicked);
    connect(btn_cancel_, &QPushButton::clicked, this, &QDialog::reject);
}

DialogAuth::~DialogAuth(){
}

void DialogAuth::on_btn_okay_clicked() {
    std::string msg;
    this->main_key_ = edit_password_->text();
    // 验证当前用户身份
    /*
    const auto auth = new zinpass::business::Authentication();
    if (auth->validateUser(msg, this->main_key_.toStdString())) {
        accept();
    }
    else {
        QMessageBox::warning(this, QString("验证失败"), QString("密码错误"));
    }
    delete sysUsrMgr;*/
}

QString DialogAuth::get_input_main_key() const{
    return this->main_key_;
}
