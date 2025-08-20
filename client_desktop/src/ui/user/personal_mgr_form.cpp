#include "user/personal_mgr_form.h"
#include <QMessageBox>

#include "state/state_manager.h"
#include "rpc/user_rpc.h"

PersonalMgrForm::PersonalMgrForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

PersonalMgrForm::~PersonalMgrForm(){
}

void PersonalMgrForm::get_personal_info() {
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    const auto* user_rpc = new zinpass::rpc::UserRPC(channel);
    const auto [user_id, username, nickname, created_at, updated_at]
        = user_rpc->get_user(session_id);
    if (!user_id.empty()) {
        this->user_id_ = user_id;
        this->username_ = username;
        this->nickname_ = nickname;
        this->created_at_ = created_at;
        this->updated_at_ = updated_at;
    } else {
        QMessageBox::warning(this, "提示", "获取用户信息失败！");
    }
}

void PersonalMgrForm::setup_ui() {
    this->l_title_ = new QLabel("更新个人信息", this);
    this->l_id_ = new QLabel("ID", this);
    this->l_username_ = new QLabel("用户名", this);
    this->l_nickname_ = new QLabel("昵称", this);
    this->l_created_at_ = new QLabel("创建时间", this);
    this->l_updated_at_ = new QLabel("更新时间", this);
    this->l_old_password_ = new QLabel("旧密码", this);
    this->l_new_password_ = new QLabel("新密码", this);
    this->l_confirm_password_ = new QLabel("确认密码", this);

    this->e_id_ = new QLineEdit(this);
    this->e_id_->setReadOnly(true);
    this->e_id_->setMaximumWidth(300);
    this->e_username_ = new QLineEdit(this);
    this->e_username_->setMaximumWidth(300);
    this->e_nickname_ = new QLineEdit(this);
    this->e_nickname_->setMaximumWidth(300);
    this->e_created_at_ = new QLineEdit(this);
    this->e_created_at_->setReadOnly(true);
    this->e_created_at_->setMaximumWidth(300);
    this->e_updated_at_ = new QLineEdit(this);
    this->e_updated_at_->setReadOnly(true);
    this->e_updated_at_->setMaximumWidth(300);
    this->e_old_password_ = new QLineEdit(this);
    this->e_old_password_->setPlaceholderText("请输入当前主密钥");
    this->e_old_password_->setEchoMode(QLineEdit::Password);
    this->e_old_password_->setMaximumWidth(300);
    this->e_new_password_ = new QLineEdit(this);
    this->e_new_password_->setPlaceholderText("请输入新密码");
    this->e_new_password_->setEchoMode(QLineEdit::Password);
    this->e_new_password_->setMaximumWidth(300);
    this->e_confirm_password_ = new QLineEdit(this);
    this->e_confirm_password_->setPlaceholderText("请再次输入新密码");
    this->e_confirm_password_->setEchoMode(QLineEdit::Password);
    this->e_confirm_password_->setMaximumWidth(300);

    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_save_ = new QPushButton("保存信息", this);
    this->btn_ch_pwd_ = new QPushButton("修改密码", this);
    this->btn_cancel_acc_ = new QPushButton("注销账号", this);

    this->e_info_ = new QTextEdit(this);
    this->e_info_->setHtml("<h3>说明</h3>"
                           "<p>1、主密钥功能包括：登录系统、验证身份、加密系统密钥。</p>"
                           "<p>2、更新主密钥：变更登录数据、重新加密当前所有用户的相应数据。</p>");
    this->e_info_->setReadOnly(true);

    // 构造布局对象
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_bottom_ = new QHBoxLayout(this);
    this->lyt_base_info_ = new QFormLayout(this);
    this->lyt_pwd_ = new QFormLayout(this);
    this->lyt_btn_ = new QVBoxLayout(this);

    this->box_info_ = new QGroupBox("基本信息",this);
    this->box_info_->setLayout(lyt_base_info_);
    this->box_pwd_ = new QGroupBox("密码密钥", this);
    this->box_pwd_->setLayout(lyt_pwd_);

    // 布局设置
    lyt_base_info_->addRow(this->l_id_, this->e_id_);
    lyt_base_info_->addRow(this->l_username_, this->e_username_);
    lyt_base_info_->addRow(this->l_nickname_, this->e_nickname_);
    lyt_base_info_->addRow(this->l_created_at_, this->e_created_at_);
    lyt_base_info_->addRow(this->l_updated_at_, this->e_updated_at_);

    lyt_pwd_->addRow(this->l_old_password_, this->e_old_password_);
    lyt_pwd_->addRow(this->l_new_password_, this->e_new_password_);
    lyt_pwd_->addRow(this->l_confirm_password_, this->e_confirm_password_);

    lyt_btn_->addWidget(this->btn_refresh_);
    lyt_btn_->addWidget(this->btn_save_);
    lyt_btn_->addWidget(this->btn_ch_pwd_);
    lyt_btn_->addWidget(this->btn_cancel_acc_);
    lyt_bottom_->addWidget(this->box_info_);
    lyt_bottom_->addWidget(this->box_pwd_);
    lyt_bottom_->addLayout(lyt_btn_);
    lyt_main_->addWidget(this->l_title_);
    lyt_main_->addWidget(this->e_info_);
    lyt_main_->addLayout(lyt_bottom_);

    // 信号与槽连接
    connect(this->btn_refresh_, &QPushButton::clicked, this, &PersonalMgrForm::on_btn_refresh_clicked);
    connect(this->btn_save_, &QPushButton::clicked, this, &PersonalMgrForm::on_btn_save_clicked);
    connect(this->btn_ch_pwd_, &QPushButton::clicked, this, &PersonalMgrForm::on_btn_ch_pwd_clicked);
    connect(this->btn_cancel_acc_, &QPushButton::clicked, this, &PersonalMgrForm::on_btn_cancel_acc_clicked);
}

void PersonalMgrForm::on_btn_refresh_clicked() {
    get_personal_info();
    this->e_id_->setText(QString::fromStdString(this->user_id_));
    this->e_username_->setText(QString::fromStdString(this->username_));
    this->e_nickname_->setText(QString::fromStdString(this->nickname_));
    this->e_created_at_->setText(QString::fromStdString(this->created_at_));
    this->e_updated_at_->setText(QString::fromStdString(this->updated_at_));
}

void PersonalMgrForm::on_btn_save_clicked() {
    // QMessageBox::information(this, "提示", "点击了保存！");
    const std::string new_username = this->e_username_->text().toStdString();
    const std::string new_nickname = this->e_nickname_->text().toStdString();

    // save new username & nickname
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    const auto* user_rpc = new zinpass::rpc::UserRPC(channel);
    if (const auto [ret, msg] =user_rpc->update_user(session_id, new_username, new_nickname);
        ret) {
        QMessageBox::information(this, "成功", QString::fromStdString(msg));
    } else {
        QMessageBox::warning(this, "失败", QString::fromStdString(msg));
    }
}

void PersonalMgrForm::on_btn_ch_pwd_clicked() {
    const std::string old_password = this->e_old_password_->text().toStdString();
    const std::string new_password = this->e_new_password_->text().toStdString();
    const std::string confirm_password = this->e_confirm_password_->text().toStdString();

    if(old_password.empty() || new_password.empty() || confirm_password.empty()) {
        QMessageBox::warning(this, "提示", "密码不能为空！");
        return;
    }
    if(new_password != confirm_password) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致！");
        return;
    }

    // update password
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    const auto* user_rpc = new zinpass::rpc::UserRPC(channel);
    const auto [ret, msg, total_count, success_count, failed_ids] = user_rpc->update_main_key(session_id, old_password, new_password);
    QString result_msg = QString::fromStdString(
            "应更新记录数：%1\n"
            "实际受影响数：%2\n"
            "失败列表：\n").arg(total_count).arg(success_count);
    for(const auto& id : failed_ids) {
        result_msg += QString::fromStdString(id) + "\n";
    }
    QMessageBox::information(this, "执行结果", result_msg);
}

void PersonalMgrForm::on_btn_cancel_acc_clicked() {
    QMessageBox::warning(this, "警告", "注销账号功能暂未实现！");
}
