#include "account/dialog_delete_account.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include "state/state_manager.h"
#include "rpc/account_rpc.h"

DialogDeleteAccount::DialogDeleteAccount(const std::string& account_id, QWidget* parent) : QDialog(parent), account_id_(account_id) {
    setup_ui();
}

DialogDeleteAccount::~DialogDeleteAccount() {

}

void DialogDeleteAccount::set_account_id(const std::string& account_id){
    this->account_id_ = account_id;
}

std::string DialogDeleteAccount::get_input_main_key() const {
    return this->main_key_;
}

void DialogDeleteAccount::on_btn_okay_clicked(){
    this->main_key_ = this->edit_password_->text().toStdString();
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 执行删除操作
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[result, message_remove] = account_rpc.remove_account(session_id, this->account_id_, this->main_key_);
    QMessageBox::information(this, QString("删除结果"), QString::fromStdString(message_remove));
    accept();
}

void DialogDeleteAccount::setup_ui(){
    setWindowTitle("删除账号");
    setFixedSize(200, 120);

    // 垂直布局管理器
    auto* lyt_main = new QVBoxLayout(this);

    // 密码标签和输入框
    auto* lyt_pwd = new QHBoxLayout();
    auto* l_password = new QLabel("主密钥", this);
    this->edit_password_ = new QLineEdit(this);
    this->edit_password_->setEchoMode(QLineEdit::Password);
    lyt_pwd->addWidget(l_password);
    lyt_pwd->addWidget(this->edit_password_);
    lyt_main->addLayout(lyt_pwd);

    // 按钮布局
    auto* lyt_btn = new QHBoxLayout();
    this->btn_okay_ = new QPushButton("确定", this);
    this->btn_cancel_ = new QPushButton("取消", this);
    lyt_btn->addWidget(this->btn_okay_);
    lyt_btn->addWidget(this->btn_cancel_);
    lyt_main->addLayout(lyt_btn);

    // 连接按钮的点击信号到槽函数
    connect(this->btn_okay_, &QPushButton::clicked, this, &DialogDeleteAccount::on_btn_okay_clicked);
    connect(this->btn_cancel_, &QPushButton::clicked, this, &QDialog::reject);
}
