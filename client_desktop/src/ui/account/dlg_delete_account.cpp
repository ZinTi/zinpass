#include "account/dlg_delete_account.h"
#include <QMessageBox>

#include "common/toast.h"
#include "state/state_manager.h"
#include "state/channel_manager.h"
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

void DialogDeleteAccount::on_btn_confirm_clicked(){
    this->main_key_ = edit_main_key_->text().toStdString();
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 执行删除操作
    zinpass::rpc::AccountRPC account_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[result, message_remove] = account_rpc.remove_account(session_id, this->account_id_, this->main_key_);

    if (result) {
        Toast::showToast(this, QString::fromStdString("删除成功: %1").arg(message_remove), 3000,QColor(200, 0, 200, 220), Qt::white);
        accept();  // 删除成功，关闭对话框并返回Accepted
    } else {  // 操作失败时保持对话框打开
        Toast::showToast(this, QString::fromStdString("删除失败: %2").arg(message_remove), 3000,QColor(200, 0, 200, 220), Qt::white);
    }
    emit remove_result(result); // 发送结果信号
}

void DialogDeleteAccount::setup_ui(){
    setWindowTitle("删除账号");
    setFixedSize(200, 120);

    // 垂直布局管理器
    lyt_main_ = new QVBoxLayout(this);

    // 密码标签和输入框
    lyt_pwd_ = new QHBoxLayout(this);
    lbl_main_key_ = new QLabel("主密钥", this);
    edit_main_key_ = new QLineEdit(this);
    edit_main_key_->setEchoMode(QLineEdit::Password);
    lyt_pwd_->addWidget(lbl_main_key_);
    lyt_pwd_->addWidget(edit_main_key_);
    lyt_main_->addLayout(lyt_pwd_);

    // 按钮布局
    lyt_btn_ = new QHBoxLayout(this);
    this->btn_confirm_ = new QPushButton("确定", this);
    btn_cancel_ = new QPushButton("取消", this);
    lyt_btn_->addWidget(btn_confirm_);
    lyt_btn_->addWidget(btn_cancel_);
    lyt_main_->addLayout(lyt_btn_);

    connect(btn_confirm_, &QPushButton::clicked, this, &DialogDeleteAccount::on_btn_confirm_clicked);
    connect(btn_cancel_, &QPushButton::clicked, this, &QDialog::reject);
}
