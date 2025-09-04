#include "account/dlg_exposed_pwd.h"
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include "state/state_manager.h"
#include "state/channel_manager.h"
#include "rpc/account_rpc.h"

DialogExposedPwd::DialogExposedPwd(const std::string& account_id, QWidget* parent)
    : QDialog(parent), account_id_(account_id) {
    setup_ui();
}

DialogExposedPwd::~DialogExposedPwd() {
}

void DialogExposedPwd::setup_ui(){
    this->setMaximumSize(360, 200);
    lyt_main_ = new QVBoxLayout(this);
    lyt_main_key_ = new QFormLayout(this);
    lyt_btn_ = new QHBoxLayout(this);
    edit_display_ = new QTextEdit();
    edit_display_->setMaximumHeight(100);
    edit_display_->setReadOnly(true);
    edit_main_key_ = new QLineEdit();
    edit_main_key_->setEchoMode(QLineEdit::Password);
    edit_main_key_->setPlaceholderText(QString("主密钥"));
    btn_read_ = new QPushButton(QString("查看"));
    btn_read_->setDefault(true);
    btn_copy_ = new QPushButton(QString("复制"));
    btn_read_and_copy_ = new QPushButton(QString("查看并复制"));
    lyt_main_key_->addRow(QString("密钥"), edit_main_key_);
    lyt_btn_->addWidget(btn_read_);
    lyt_btn_->addWidget(btn_copy_);
    lyt_btn_->addWidget(btn_read_and_copy_);
    lyt_main_->setContentsMargins(QMargins(22,18,22,18));
    lyt_main_->addWidget(edit_display_);
    lyt_main_->addLayout(lyt_main_key_);
    lyt_main_->addLayout(lyt_btn_);

    this->timer_ = new QTimer(this);
    connect(this->timer_, &QTimer::timeout, this, &DialogExposedPwd::update_button);

    connect(btn_read_, &QPushButton::clicked, this, &DialogExposedPwd::on_btn_read_clicked);
    connect(btn_copy_, &QPushButton::clicked, this, &DialogExposedPwd::on_btn_copy_clicked);
    connect(btn_read_and_copy_, &QPushButton::clicked, this, &DialogExposedPwd::on_btn_read_and_copy_clicked);
}

void DialogExposedPwd::start_btn_countdown(QPushButton* btn) {
    this->current_btn_ = btn;
    this->original_btn_text_ = btn->text();
    this->countdown_ = 6;
    this->current_btn_->setText(QString("%1 秒").arg(this->countdown_));
    btn_read_->setEnabled(false);
    btn_read_and_copy_->setEnabled(false);
    this->timer_->start(1000);
}

void DialogExposedPwd::update_button() {
    this->countdown_--;
    if (this->countdown_ > 0) {
        this->current_btn_->setText(QString("%1 秒").arg(this->countdown_));
    } else {
        this->timer_->stop();
        this->current_btn_->setText(this->original_btn_text_);
        btn_read_->setEnabled(true);
        btn_read_and_copy_->setEnabled(true);
        this->current_btn_ = nullptr;
        edit_display_->setTextColor(QColor::fromRgb(120, 120, 255));
        edit_display_->setText(QString("倒计时结束"));
    }
}

void DialogExposedPwd::on_btn_read_clicked() {
    edit_display_->clear();
    edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
    edit_display_->append(QString("[操作] 仅查看密码"));

    const std::string in_main_key = edit_main_key_->text().toStdString();
    edit_main_key_->clear(); // 读取后清除
    if(in_main_key.empty()){
        edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        edit_display_->append(QString("[必需] 密钥为空，请输入主密钥"));
        return;
    }

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    zinpass::rpc::AccountRPC account_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[plaintext_password, message] = account_rpc.fetch_password(session_id, this->account_id_, in_main_key);

    if(plaintext_password.empty()){
        edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        edit_display_->append(QString("[响应] %1").arg(message));
        edit_display_->append(QString("[密码] (空)"));
    }else{
        edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
        edit_display_->append(QString("[响应] %1").arg(message));
        edit_display_->append(QString("[密码] %1").arg(plaintext_password));

        start_btn_countdown(btn_read_);   // 成功获取到密码则开始倒计时
    }
}

void DialogExposedPwd::on_btn_copy_clicked() const {
    edit_display_->clear();
    edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
    edit_display_->append(QString("[操作] 仅复制密码"));

    const std::string in_main_key = edit_main_key_->text().toStdString();
    edit_main_key_->clear(); // 读取后清除
    if(in_main_key.empty()){
        edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        edit_display_->append(QString("[必需] 密钥为空，请输入主密钥"));
        return;
    }

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    zinpass::rpc::AccountRPC account_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[plaintext_password, message] = account_rpc.fetch_password(session_id, this->account_id_, in_main_key);

    if(plaintext_password.empty()){
        edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        edit_display_->append(QString("[响应] %1").arg(message));
        edit_display_->append(QString("[复制] 未复制"));
    }else{
        edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
        edit_display_->append(QString("[响应] %1").arg(message));
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(QString::fromStdString(plaintext_password));
        edit_display_->append(QString("[复制] 已复制到剪贴板"));
    }
}

void DialogExposedPwd::on_btn_read_and_copy_clicked(){
    edit_display_->clear();
    edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
    edit_display_->append(QString("[操作] 查看密码并复制到剪贴板"));

    const std::string in_main_key = edit_main_key_->text().toStdString();
    edit_main_key_->clear(); // 读取后清除
    if(in_main_key.empty()){
        edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        edit_display_->append(QString("[必需] 密钥为空，请输入主密钥"));
        return;
    }

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    zinpass::rpc::AccountRPC account_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[plaintext_password, message] = account_rpc.fetch_password(session_id, this->account_id_, in_main_key);

    if(plaintext_password.empty()){
        edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        edit_display_->append(QString("[响应] %1").arg(message));
        edit_display_->append(QString("[密码] (空)"));
        edit_display_->append(QString("[复制] 未复制"));
    }else{
        edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
        edit_display_->append(QString("[响应] %1").arg(message));
        edit_display_->append(QString("[密码] %1").arg(plaintext_password));
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(QString::fromStdString(plaintext_password));
        edit_display_->append(QString("[复制] 已复制到剪贴板"));
        start_btn_countdown(btn_read_and_copy_);  // 成功获取到密码则开始倒计时
    }
}
