#include "account/dlg_edit_account.h"
#include "common/dlg_auth.h"
#include "state/state_manager.h"
#include "state/channel_manager.h"
#include <QMessageBox>
#include "dlg_delete_account.h"
// RPC 封装
#include "rpc/account_rpc.h"
#include "rpc/phone_rpc.h"
#include "rpc/category_rpc.h"

DialogEditAccount::DialogEditAccount(const std::string& account_id, QWidget* parent) : QDialog(parent), account_id_(account_id) {
    // 创建主窗口的中心部件
    setup_ui();
    initial_input_widgets(); // 初始化输入控件
}

DialogEditAccount::~DialogEditAccount() {
}

void DialogEditAccount::setup_ui(){
    // 主布局
    // QWidget* central_widget = new QWidget(this);    // 表单中心部件
    lyt_main_ = new QVBoxLayout(this);
    lyt_grid_ = new QGridLayout(this); // 创建网格布局
    lyt_key_ = new QFormLayout(this); // 创建主密钥布局
    lyt_action_ = new QHBoxLayout(this);
    lyt_main_->addSpacing(30);
    lyt_main_->addLayout(lyt_grid_);
    lyt_main_->addLayout(lyt_action_);
    lyt_main_->addSpacing(30);

    // 创建四个 GroupBox ---------------------------------------------------
    gb_platform_ = new QGroupBox("平台信息", this);
    gb_account_ = new QGroupBox("账号信息", this);
    gb_third_party_ = new QGroupBox("第三方账号绑定", this);
    gb_other_ = new QGroupBox("其他信息", this);

    // 将 GroupBox 添加到网格布局
    lyt_grid_->addWidget(gb_platform_, 0, 0);    // 第一行第一列
    lyt_grid_->addWidget(gb_account_, 0, 1);   // 第一行第二列
    lyt_grid_->addWidget(gb_third_party_, 1, 0); // 第二行第一列
    lyt_grid_->addWidget(gb_other_, 1, 1);// 第二行第二列

    // 为第一个 GroupBox 添加表单布局并创建控件------------------------------
    lyt_top_left_ = new QFormLayout(gb_platform_);
    edit_provider_name_ = new QLineEdit(gb_platform_);
    edit_platform_name_ = new QLineEdit(gb_platform_);
    edit_url_ = new QLineEdit(gb_platform_);
    combo_category_= new QComboBox(gb_account_);
    edit_hotline_ = new QLineEdit(gb_platform_);
    lyt_top_left_->addRow("服务商", edit_provider_name_);
    lyt_top_left_->addRow("平台名", edit_platform_name_);
    lyt_top_left_->addRow("地址", edit_url_);
    lyt_top_left_->addRow("分类", combo_category_);
    lyt_top_left_->addRow("客服热线", edit_hotline_);

    // 为第二个 GroupBox 添加表单布局并创建控件------------------------------
    lyt_top_right_ = new QFormLayout(gb_account_);
    edit_username_ = new QLineEdit(gb_account_);
    edit_nickname_ = new QLineEdit(gb_account_);
    edit_password_ = new QLineEdit(gb_account_);
    edit_password_->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    edit_sub_account_ = new QLineEdit(this);
    lyt_top_right_->addRow("用户名", edit_username_);
    lyt_top_right_->addRow("昵称", edit_nickname_);
    lyt_top_right_->addRow("密码", edit_password_);
    lyt_top_right_->addRow("子账号", edit_sub_account_);


    // 为第三个 GroupBox 添加表单布局并创建控件------------------------------
    lyt_bottom_left_ = new QFormLayout(gb_third_party_);
    combo_phone_ = new QComboBox(gb_third_party_);
    combo_email_ = new QComboBox(gb_third_party_);
    lyt_bottom_left_->addRow("手机号", combo_phone_);
    lyt_bottom_left_->addRow("邮箱", combo_email_);

    // 为第四个 GroupBox 添加表单布局并创建控件------------------------------
    lyt_bottom_right_ = new QFormLayout(gb_other_);
    edit_postscript_ = new QPlainTextEdit();
    lyt_bottom_right_->addRow("附言", edit_postscript_);

    // 设置占位符
    edit_provider_name_->setPlaceholderText("平台所属组织或公司");
    edit_platform_name_->setPlaceholderText("账号所属平台");
    edit_url_->setPlaceholderText("网站域名或IP地址");
    edit_hotline_->setPlaceholderText("客服电话");
    edit_username_->setPlaceholderText("可用于登录的用户名或UID");
    edit_nickname_->setPlaceholderText("昵称或真实姓名");
    edit_password_->setPlaceholderText("密码、密钥或口令 (置空不修改)");
    edit_sub_account_->setPlaceholderText("是否有同账号平台下的子账号");
    edit_postscript_->setPlaceholderText("备注");

    // 统一控件高度
    constexpr int fixed_height = 22;
    edit_provider_name_->setFixedHeight(fixed_height);
    edit_platform_name_->setFixedHeight(fixed_height);
    edit_url_->setFixedHeight(fixed_height);
    combo_category_->setFixedHeight(fixed_height);
    edit_hotline_->setFixedHeight(fixed_height);
    edit_username_->setFixedHeight(fixed_height);
    edit_nickname_->setFixedHeight(fixed_height);
    edit_password_->setFixedHeight(fixed_height);
    edit_sub_account_->setFixedHeight(fixed_height);
    combo_phone_->setFixedHeight(fixed_height);
    combo_email_->setFixedHeight(fixed_height);
    edit_postscript_->setFixedHeight(72);

    // 设置布局伸缩比例
    lyt_grid_->setRowStretch(0, 1); // 第一行占1份比例
    lyt_grid_->setRowStretch(1, 1); // 第二行占1份比例
    lyt_grid_->setColumnStretch(0, 1); // 第一列占1份
    lyt_grid_->setColumnStretch(1, 1); // 第二列占1份
    // 设置间距
    lyt_grid_->setSpacing(20);

    // 主密钥输入框布局
    edit_main_key_ = new QLineEdit(this);
    edit_main_key_->setPlaceholderText("主密钥");
    edit_main_key_->setEnabled(false);
    edit_main_key_->setEchoMode(QLineEdit::Password);
    lyt_key_->addRow("主密钥", edit_main_key_);

    // 按钮布局
    btn_refresh_ = new QPushButton("刷新", this);
    btn_read_passwd_ = new QPushButton("查看密码", this);
    btn_edit_ = new QPushButton("编辑", this);
    btn_submit_ = new QPushButton("提交", this);
    lyt_action_->addLayout(lyt_key_);
    lyt_action_->addWidget(btn_refresh_);
    lyt_action_->addWidget(btn_read_passwd_);
    lyt_action_->addWidget(btn_edit_);
    lyt_action_->addWidget(btn_submit_);

    // 连接信号槽
    connect(btn_refresh_, &QPushButton::clicked, this, &DialogEditAccount::initial_input_widgets);
    connect(btn_read_passwd_, &QPushButton::clicked, this, &DialogEditAccount::on_btn_read_passwd_clicked);
    connect(btn_edit_, &QPushButton::clicked, this, [this, b = false](){
        DialogEditAccount::read_only(b);
    });
    // connect(btn_submit_, &QPushButton::clicked, this, &DialogEditAccount::form_submitted);
    connect(btn_submit_, &QPushButton::clicked, this, &DialogEditAccount::on_btn_submit_clicked);

    // 当edit_password_不为空时启用edit_main_key_;
    connect(edit_password_, &QLineEdit::textChanged, this, &DialogEditAccount::update_edit_main_key_state);
}

void DialogEditAccount::initial_input_widgets() const {
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 获取数据：可选 phone_numbers、email_addresses、account_categories
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[phone_numbers, message_phone] = phone_rpc.list_phone_numbers(session_id);
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[email_addresses, message_email] = account_rpc.list_email_addresses(session_id);
    zinpass::rpc::CategoryRPC account_category_rpc(channel);
    const auto[name_of_categories, message_category] = account_category_rpc.list_account_categories(session_id);
    const auto[view_account, message_account] = account_rpc.list_account_by_id(session_id, this->account_id_);

    // 3. 渲染到 UI 控件
    combo_phone_->clear();
    combo_phone_->addItem(QString("无")); // default
    for(const std::string& phone_number : phone_numbers){
        combo_phone_->addItem(QString::fromStdString(phone_number));
    }

    combo_email_->clear();
    combo_email_->addItem(QString("无"));
    for (const std::string& email_address : email_addresses) {
        combo_email_->addItem(QString::fromStdString(email_address));
    }

    combo_category_->clear();
    for (const std::string& category : name_of_categories) {
        combo_category_->addItem(QString::fromStdString(category));
    }

    edit_provider_name_->setText(QString::fromStdString(view_account.getProviderName()));
    edit_platform_name_->setText(QString::fromStdString(view_account.getPlatformName()));
    edit_url_->setText(QString::fromStdString(view_account.getURL()));
    combo_category_->setCurrentText(QString::fromStdString(view_account.getCategory()));
    edit_hotline_->setText(QString::fromStdString(view_account.getHotline()));
    edit_username_->setText(QString::fromStdString(view_account.getUsername()));
    edit_nickname_->setText(QString::fromStdString(view_account.getNickname()));
    edit_password_->setStyleSheet(
        "QLineEdit { color: rgb(183, 110, 121); }"
        "QLineEdit:disabled { color: gray; }"
        );
    edit_sub_account_->setText(QString::fromStdString(view_account.getSubAccount()));
    combo_phone_->setCurrentText(QString::fromStdString(view_account.getPhone()));
    combo_email_->setCurrentText(QString::fromStdString(view_account.getEmail()));
    edit_postscript_->setPlainText(QString::fromStdString(view_account.getPostscript()));

    read_only(true); // 将所有控件设置为只读
}

void DialogEditAccount::read_only(const bool enable) const {
    btn_edit_->setEnabled( enable );
    btn_edit_->setText(enable ? "编辑" : "编辑中");
    btn_submit_->setEnabled( !enable );   // 默认提交按钮不可点击，点击编辑按钮后方可提交

    edit_provider_name_->setReadOnly(enable);
    edit_platform_name_->setReadOnly(enable);
    edit_url_->setReadOnly(enable);
    combo_category_->setEnabled( !enable );
    edit_hotline_->setReadOnly(enable);
    edit_username_->setReadOnly(enable);
    edit_nickname_->setReadOnly(enable);
    // if( !enable ) edit_password_->clear();
    edit_password_->setReadOnly(enable);
    edit_sub_account_->setReadOnly(enable);
    combo_phone_->setEnabled( !enable );
    combo_email_->setEnabled( !enable );
    edit_postscript_->setReadOnly(enable);
}

void DialogEditAccount::on_btn_read_passwd_clicked() const {
    DialogExposedPwd dlg_exposed_pwd(this->account_id_);
    dlg_exposed_pwd.setWindowTitle(QString("查看密码(%1)").arg(this->account_id_));
    dlg_exposed_pwd.exec();
}

QMap<QString, QVariant> DialogEditAccount::get_form_data() const{
    return {
        {"provider_name", edit_provider_name_->text()},
        {"platform_name", edit_platform_name_->text()},
        {"url", edit_url_->text()},
        {"category", combo_category_->currentText()},
        {"hotline", edit_hotline_->text()},
        {"username", edit_username_->text()},
        {"nickname", edit_nickname_->text()},
        {"password", edit_password_->text()},
        {"sub_account", edit_sub_account_->text()},
        {"phone", combo_phone_->currentText()},
        {"email", combo_email_->currentText()},
        {"postscript", edit_postscript_->toPlainText()}
    };
}

void DialogEditAccount::set_account_id(const std::string& account_id) {
    this->account_id_ = account_id;
}

void DialogEditAccount::on_btn_submit_clicked(){
    // 1. 获取用户输入、合法性检验
    std::string in_provider_name = edit_provider_name_->text().toStdString();
    std::string in_platform_name = edit_platform_name_->text().toStdString();
    std::string in_url = edit_url_->text().toStdString();
    std::string in_category_name = combo_category_->currentText().toStdString();
    std::string in_hotline = edit_hotline_->text().toStdString();
    std::string in_username = edit_username_->text().toStdString();
    std::string in_nickname = edit_nickname_->text().toStdString();
    std::string in_password = edit_password_->text().toStdString();
    std::string in_sub_account = edit_sub_account_->text().toStdString();
    std::string in_phone = combo_phone_->currentText().toStdString();
    std::string in_email = combo_email_->currentText().toStdString();
    std::string in_postscript = edit_postscript_->toPlainText().toStdString();


    // 2. 验证身份并获取主密钥
    std::string password_key;
    /*
    if(false == in_password.empty()){ // 不更新密码时，不用验证身份
        DialogAuth dlg_auth(this);
        if(dlg_auth.exec() == QDialog::Accepted){
            password_key = dlg_auth.get_input_main_key().toStdString();
        }else{
            return;
        }
    }*/
    if(false == in_password.empty()){
        password_key = edit_main_key_->text().toStdString();
        if(password_key.empty()){
            QMessageBox::warning(this, QString("无法更新"), QString("你的操作需要更新账号的密码\n因此主密钥输入框不能为空！"));
            return;
        }
    }

    // 3. 更新记录
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    zinpass::rpc::AccountRPC account_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[result, message_update] = account_rpc.update_account(
        session_id,
        this->account_id_,
        in_provider_name,
        in_platform_name,
        in_username,
        in_nickname,
        in_sub_account,
        in_phone,
        in_email,
        in_postscript,
        in_url,
        in_hotline,
        in_category_name,
        in_password,
        password_key
        );
    if(result){
        QMessageBox::information(this, QString("成功"), QString::fromStdString(message_update));
    }else{
        QMessageBox::warning(this, QString("失败"), QString::fromStdString(message_update));
    }
}

void DialogEditAccount::update_edit_main_key_state() const {
    const bool isEditPasswordEmpty = edit_password_->text().trimmed().isEmpty();

    if (isEditPasswordEmpty) {
        edit_main_key_->clear();
        edit_main_key_->setEnabled(false); // 强制禁用 edit_main_key_
    } else {
        edit_main_key_->setEnabled(true);  // 只要 edit_password_ 有内容就启用 edit_main_key_
    }
}
