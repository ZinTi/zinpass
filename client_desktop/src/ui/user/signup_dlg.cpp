#include "user/signup_dlg.h"
#include <QMessageBox>
#include "common/is_input_valid.h"
#include "rpc/user_rpc.h"
#include <string>
#include "state/channel_manager.h"

SignupDlg::SignupDlg(QWidget* parent) : QDialog(parent) {
    setup_ui();

}

SignupDlg::~SignupDlg() {
    // 所有UI控件在创建时都指定父对象（this），它们会被Qt的对象树自动管理。
    // 当父对象（SignupDlg）被销毁时，这些子对象会自动删除。
}

void SignupDlg::setup_ui() {
    const QString style_edit("background-color: #FFDC73; color: black;");
    const QString style_btn("background-color: #FFCF40;color: black;");
    const QString style_common("color: black;");
    const QFont font_btn("Microsoft YaHei UI", 12);

    this->setWindowTitle("注册 | ZinPass");
    this->setFixedSize(300, 354);
    this->setStyleSheet("background-color: #FFBF00;");

    l_title_ = new QLabel("注册新用户", this);
    const QFont font_title("字酷堂石刻体", 20);
    l_title_->setFont(font_title);
    l_title_->setStyleSheet(style_common);
    l_title_->setAlignment(Qt::AlignCenter);

    l_username_ = new QLabel("用户名", this);
    l_username_->setStyleSheet(style_common);
    l_passwd1_ = new QLabel("设置密码", this);
    l_passwd1_->setStyleSheet(style_common);
    l_passwd2_ = new QLabel("确定密码", this);
    l_passwd2_->setStyleSheet(style_common);
    l_nickname_ = new QLabel("昵称", this);
    l_nickname_->setStyleSheet(style_common);
    e_username_ = new QLineEdit(this);
    e_username_->setMaxLength(16);
    e_username_->setStyleSheet(style_edit);
    e_passwd1_ = new QLineEdit(this);
    e_passwd1_->setMaxLength(50);
    e_passwd1_->setStyleSheet(style_edit);
    e_passwd1_->setEchoMode(QLineEdit::Password);
    e_passwd2_ = new QLineEdit(this);
    e_passwd2_->setMaxLength(50);
    e_passwd2_->setStyleSheet(style_edit);
    e_passwd2_->setEchoMode(QLineEdit::Password);
    e_nickname_ = new QLineEdit(this);
    e_nickname_->setMaxLength(12);
    e_nickname_->setStyleSheet(style_edit);

    btn_clear_ = new QPushButton("清空输入",  this);
    btn_clear_->setStyleSheet(style_btn);
    btn_clear_->setFont(font_btn);
    btn_clear_->setFixedWidth(100);
    btn_submit_ = new QPushButton("确定注册", this);
    btn_submit_->setStyleSheet(style_btn);
    btn_submit_->setFont(font_btn);
    btn_submit_->setFixedWidth(100);
    btn_submit_->setDefault(true);

    gb_necessary_ = new QGroupBox("核心项", this);
    gb_other_ = new QGroupBox("扩展项", this);
    lyt_main_ = new QVBoxLayout(this);
    lyt_necessary_ = new QFormLayout(this);
    lyt_other_ = new QFormLayout(this);
    lyt_btn_ = new QHBoxLayout(this);

    lyt_necessary_->addRow(l_username_, e_username_);
    lyt_necessary_->addRow(l_passwd1_, e_passwd1_);
    lyt_necessary_->addRow(l_passwd2_, e_passwd2_);
    lyt_other_->addRow(l_nickname_, e_nickname_);

    lyt_btn_->addWidget(btn_clear_);
    lyt_btn_->addWidget(btn_submit_);

    gb_necessary_->setLayout(lyt_necessary_);
    gb_other_->setLayout(lyt_other_);

    lyt_main_->setContentsMargins(28, 10, 30, 28);
    lyt_main_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    lyt_main_->addWidget(l_title_);
    lyt_main_->addWidget(gb_necessary_);
    lyt_main_->addWidget(gb_other_);
    lyt_main_->addStretch();
    lyt_main_->addLayout(lyt_btn_);

    connect(btn_clear_, QPushButton::clicked, this, SignupDlg::on_btn_clear_clicked);
    connect(btn_submit_, QPushButton::clicked, this, SignupDlg::on_btn_submit_clicked);
}

void SignupDlg::on_btn_submit_clicked() {
    // 1. 获取控件输入并初步检验数据合法性
    const QString in_username = e_username_->text();
    const QString in_pwd1 = e_passwd1_->text();
    const QString in_pwd2 = e_passwd2_->text();
    const QString in_nickname = e_nickname_->text();
    if (false == isUsernameValid(in_username)) return;
    if (false == isPasswordValid(in_pwd1, in_pwd2)) return;
    if (false == isNicknameValid(in_nickname)) return;
    // 2. 执行注册操作
    const zinpass::rpc::UserRPC user_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[opt_data, message] = user_rpc.register_account(
        in_username.toStdString(),
        in_pwd1.toStdString(),
        in_nickname.toStdString()
        );
    // 3. 弹窗显示结果
    if(opt_data == std::nullopt){
        QMessageBox::critical(this, QString("错误"), QString::fromStdString(message));
    } else {
        QMessageBox::information(this, QString("注册结果"), QString::fromStdString(message) + QString::fromStdString(", id: " + opt_data.value()));
    }
}

void SignupDlg::on_btn_clear_clicked() const{
    e_nickname_->clear();
    e_username_->clear();
    e_passwd1_->clear();
    e_passwd2_->clear();
}

