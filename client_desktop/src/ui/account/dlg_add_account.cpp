#include "account/dlg_add_account.h"
#include "state/state_manager.h"
#include "state/channel_manager.h"
#include "common/dlg_auth.h"
#include <QMessageBox>

// RPC 封装
#include "rpc/account_rpc.h"
#include "rpc/phone_rpc.h"
#include "rpc/category_rpc.h"

DialogAddAccount::DialogAddAccount(QWidget *parent) : QDialog(parent){
    setup_ui();
}

DialogAddAccount::~DialogAddAccount(){
}

void DialogAddAccount::setup_ui(){
    // 主布局
    // QWidget* central_widget = new QWidget(this);    // 表单中心部件
    lyt_main_ = new QVBoxLayout(this);
    lyt_grid_ = new QGridLayout(this); // 创建网格布局
    lyt_form_key_ = new QFormLayout(this);
    lyt_key_and_btn_ = new QHBoxLayout(this);
    lyt_main_->addSpacing(30);
    lyt_main_->addLayout(lyt_grid_);
    lyt_main_->addLayout(lyt_key_and_btn_);
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
    edit_password_->setPlaceholderText("密码、密钥或口令");
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

    // 主密钥
    edit_main_key_ = new QLineEdit(this);
    edit_main_key_->setPlaceholderText("主密钥");
    edit_main_key_->setEchoMode(QLineEdit::Password);
    lyt_form_key_->addRow("密钥", edit_main_key_);

    // 按钮布局
    btn_refresh_ = new QPushButton("刷新", this);
    btn_refresh_->setMaximumWidth(60);
    btn_clear_ = new QPushButton("清空", this);
    btn_clear_->setMaximumWidth(60);
    btn_submit_ = new QPushButton("提交", this);
    btn_submit_->setMaximumWidth(60);
    btn_submit_->setDefault(true);
    lyt_key_and_btn_->addLayout(lyt_form_key_);
    lyt_key_and_btn_->addWidget(btn_refresh_);
    lyt_key_and_btn_->addWidget(btn_clear_);
    lyt_key_and_btn_->addWidget(btn_submit_);

    initial_input_widgets(); // 初始化输入控件 (这一句有 bug, 初始化后并没有显示数据）

    // 连接信号槽
    connect(btn_refresh_, &QPushButton::clicked, this, &DialogAddAccount::initial_input_widgets);
    connect(btn_clear_, &QPushButton::clicked, this, &DialogAddAccount::on_btn_clear_clicked);
    connect(btn_submit_, &QPushButton::clicked, this, &DialogAddAccount::on_btn_submit_clicked);
}

void DialogAddAccount::initial_input_widgets() const{
    // 0. 清空原有数据
    edit_provider_name_->clear();
    edit_platform_name_->clear();
    edit_url_->clear();
    edit_hotline_->clear();
    edit_username_->clear();
    edit_nickname_->clear();
    edit_password_->clear();
    edit_sub_account_->clear();
    edit_postscript_->clear();

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
}

void DialogAddAccount::on_btn_clear_clicked() const {
    edit_provider_name_->clear();
    edit_platform_name_->clear();
    edit_url_->clear();
    edit_hotline_->clear();
    edit_username_->clear();
    edit_nickname_->clear();
    edit_password_->clear();
    edit_sub_account_->clear();
    edit_postscript_->clear();

    combo_category_->setCurrentIndex(0);
    combo_email_->setCurrentIndex(0);
    combo_phone_->setCurrentIndex(0);
}

void DialogAddAccount::on_btn_submit_clicked(){
    // 0. 空数据检测

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

    std::string in_main_key = edit_main_key_->text().toStdString();

    // 2. 数据合法性验证

    // 3. 新增记录
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    zinpass::rpc::AccountRPC account_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[result, message_update] = account_rpc.add_account(
        session_id,
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
        in_main_key
        );
    if(result){
        QMessageBox::information(this, QString("成功"), QString::fromStdString(message_update));
    }else{
        QMessageBox::warning(this, QString("失败"), QString::fromStdString(message_update));
    }
}
