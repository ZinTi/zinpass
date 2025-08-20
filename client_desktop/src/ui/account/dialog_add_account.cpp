#include "account/dialog_add_account.h"
#include "state/state_manager.h"
#include "common/dialog_auth.h"
#include <QMessageBox>

// RPC 封装
#include "rpc/account_rpc.h"
#include "rpc/phone_rpc.h"
#include "rpc/category_rpc.h"

DialogAddAccount::DialogAddAccount(QWidget *parent) : QDialog(parent){
    setup_ui();
    initial_input_widgets(); // 初始化输入控件
}

DialogAddAccount::~DialogAddAccount(){
}

void DialogAddAccount::setup_ui(){
    // 主布局
    // QWidget* central_widget = new QWidget(this);    // 表单中心部件
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_grid_ = new QGridLayout(this); // 创建网格布局
    this->lyt_form_key_ = new QFormLayout(this);
    this->lyt_key_and_btn_ = new QHBoxLayout(this);
    this->lyt_main_->addSpacing(30);
    this->lyt_main_->addLayout(this->lyt_grid_);
    this->lyt_main_->addLayout(this->lyt_key_and_btn_);
    this->lyt_main_->addSpacing(30);

    // 创建四个 GroupBox ---------------------------------------------------
    this->group_box_platform_ = new QGroupBox("平台信息", this);
    this->group_box_account_ = new QGroupBox("账号信息", this);
    this->group_box_third_party_ = new QGroupBox("第三方账号绑定", this);
    this->group_box_other_ = new QGroupBox("其他信息", this);

    // 将 GroupBox 添加到网格布局
    this->lyt_grid_->addWidget(this->group_box_platform_, 0, 0);    // 第一行第一列
    this->lyt_grid_->addWidget(this->group_box_account_, 0, 1);   // 第一行第二列
    this->lyt_grid_->addWidget(this->group_box_third_party_, 1, 0); // 第二行第一列
    this->lyt_grid_->addWidget(this->group_box_other_, 1, 1);// 第二行第二列

    // 为第一个 GroupBox 添加表单布局并创建控件------------------------------
    this->lyt_top_left_ = new QFormLayout(this->group_box_platform_);
    this->edit_provider_name_ = new QLineEdit(this->group_box_platform_);
    this->edit_platform_name_ = new QLineEdit(this->group_box_platform_);
    this->edit_url_ = new QLineEdit(this->group_box_platform_);
    this->combo_category_= new QComboBox(this->group_box_account_);
    this->edit_hotline_ = new QLineEdit(this->group_box_platform_);
    this->lyt_top_left_->addRow("服务商", this->edit_provider_name_);
    this->lyt_top_left_->addRow("平台名", this->edit_platform_name_);
    this->lyt_top_left_->addRow("地址", this->edit_url_);
    this->lyt_top_left_->addRow("分类", this->combo_category_);
    this->lyt_top_left_->addRow("客服热线", this->edit_hotline_);

    // 为第二个 GroupBox 添加表单布局并创建控件------------------------------
    this->lyt_top_right_ = new QFormLayout(this->group_box_account_);
    this->edit_username_ = new QLineEdit(this->group_box_account_);
    this->edit_nickname_ = new QLineEdit(this->group_box_account_);
    this->edit_password_ = new QLineEdit(this->group_box_account_);
    this->edit_password_->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    this->edit_sub_account_ = new QLineEdit(this);
    this->lyt_top_right_->addRow("用户名", this->edit_username_);
    this->lyt_top_right_->addRow("昵称", this->edit_nickname_);
    this->lyt_top_right_->addRow("密码", this->edit_password_);
    this->lyt_top_right_->addRow("子账号", this->edit_sub_account_);

    // 为第三个 GroupBox 添加表单布局并创建控件------------------------------
    this->lyt_bottom_left_ = new QFormLayout(this->group_box_third_party_);
    this->combo_phone_ = new QComboBox(this->group_box_third_party_);
    this->combo_email_ = new QComboBox(this->group_box_third_party_);
    this->lyt_bottom_left_->addRow("手机号", this->combo_phone_);
    this->lyt_bottom_left_->addRow("邮箱", this->combo_email_);

    // 为第四个 GroupBox 添加表单布局并创建控件------------------------------
    this->lyt_bottom_right_ = new QFormLayout(this->group_box_other_);
    this->edit_postscript_ = new QPlainTextEdit();
    this->lyt_bottom_right_->addRow("附言", this->edit_postscript_);

    // 设置占位符
    this->edit_provider_name_->setPlaceholderText("平台所属组织或公司");
    this->edit_platform_name_->setPlaceholderText("账号所属平台");
    this->edit_url_->setPlaceholderText("网站域名或IP地址");
    this->edit_hotline_->setPlaceholderText("客服电话");
    this->edit_username_->setPlaceholderText("可用于登录的用户名或UID");
    this->edit_nickname_->setPlaceholderText("昵称或真实姓名");
    this->edit_password_->setPlaceholderText("密码、密钥或口令");
    this->edit_sub_account_->setPlaceholderText("是否有同账号平台下的子账号");
    this->edit_postscript_->setPlaceholderText("备注");

    // 统一控件高度
    constexpr int fixed_height = 22;
    this->edit_provider_name_->setFixedHeight(fixed_height);
    this->edit_platform_name_->setFixedHeight(fixed_height);
    this->edit_url_->setFixedHeight(fixed_height);
    this->combo_category_->setFixedHeight(fixed_height);
    this->edit_hotline_->setFixedHeight(fixed_height);
    this->edit_username_->setFixedHeight(fixed_height);
    this->edit_nickname_->setFixedHeight(fixed_height);
    this->edit_password_->setFixedHeight(fixed_height);
    this->edit_sub_account_->setFixedHeight(fixed_height);
    this->combo_phone_->setFixedHeight(fixed_height);
    this->combo_email_->setFixedHeight(fixed_height);
    this->edit_postscript_->setFixedHeight(72);

    // 设置布局伸缩比例
    this->lyt_grid_->setRowStretch(0, 1); // 第一行占1份比例
    this->lyt_grid_->setRowStretch(1, 1); // 第二行占1份比例
    this->lyt_grid_->setColumnStretch(0, 1); // 第一列占1份
    this->lyt_grid_->setColumnStretch(1, 1); // 第二列占1份
    // 设置间距
    this->lyt_grid_->setSpacing(20);

    // 主密钥
    this->edit_main_key_ = new QLineEdit(this);
    this->edit_main_key_->setPlaceholderText("主密钥");
    this->edit_main_key_->setEchoMode(QLineEdit::Password);
    this->lyt_form_key_->addRow("密钥", this->edit_main_key_);

    // 按钮布局
    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_refresh_->setMaximumWidth(60);
    this->btn_clear_ = new QPushButton("清空", this);
    this->btn_clear_->setMaximumWidth(60);
    this->btn_submit_ = new QPushButton("提交", this);
    this->btn_submit_->setMaximumWidth(60);
    this->btn_submit_->setDefault(true);
    this->lyt_key_and_btn_->addLayout(this->lyt_form_key_);
    this->lyt_key_and_btn_->addWidget(this->btn_refresh_);
    this->lyt_key_and_btn_->addWidget(this->btn_clear_);
    this->lyt_key_and_btn_->addWidget(this->btn_submit_);

    // 连接信号槽
    connect(this->btn_refresh_, &QPushButton::clicked, this, &DialogAddAccount::on_btn_refresh_clicked);
    connect(this->btn_clear_, &QPushButton::clicked, this, &DialogAddAccount::on_btn_clear_clicked);
    connect(this->btn_submit_, &QPushButton::clicked, this, &DialogAddAccount::on_btn_submit_clicked);
}

void DialogAddAccount::initial_input_widgets() const{
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 创建gRPC通道连接到服务器获取数据：可选 phone_numbers、email_addresses、account_categories
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[phone_numbers, message_phone] = phone_rpc.list_phone_numbers(session_id);
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[email_addresses, message_email] = account_rpc.list_email_addresses(session_id);
    zinpass::rpc::CategoryRPC account_category_rpc(channel);
    const auto[name_of_categories, message_category] = account_category_rpc.list_account_categories(session_id);

    // 3. 渲染到 UI 控件
    this->combo_phone_->clear();
    this->combo_phone_->addItem(QString("无")); // default
    for(const std::string& phone_number : phone_numbers){
        this->combo_phone_->addItem(QString::fromStdString(phone_number));
    }

    this->combo_email_->clear();
    this->combo_email_->addItem(QString("无"));
    for (const std::string& email_address : email_addresses) {
        this->combo_email_->addItem(QString::fromStdString(email_address));
    }

    this->combo_category_->clear();
    for (const std::string& category : name_of_categories) {
        this->combo_category_->addItem(QString::fromStdString(category));
    }
}


void DialogAddAccount::on_btn_refresh_clicked() const {
    this->edit_provider_name_->clear();
    this->edit_platform_name_->clear();
    this->edit_url_->clear();
    this->edit_hotline_->clear();
    this->edit_username_->clear();
    this->edit_nickname_->clear();
    this->edit_password_->clear();
    this->edit_sub_account_->clear();
    this->edit_postscript_->clear();

    initial_input_widgets();
}

void DialogAddAccount::on_btn_clear_clicked() const {
    this->edit_provider_name_->clear();
    this->edit_platform_name_->clear();
    this->edit_url_->clear();
    this->edit_hotline_->clear();
    this->edit_username_->clear();
    this->edit_nickname_->clear();
    this->edit_password_->clear();
    this->edit_sub_account_->clear();
    this->edit_postscript_->clear();

    this->combo_category_->setCurrentIndex(0);
    this->combo_email_->setCurrentIndex(0);
    this->combo_phone_->setCurrentIndex(0);
}

void DialogAddAccount::on_btn_submit_clicked(){
    // 0. 空数据检测

    // 1. 获取用户输入、合法性检验
    std::string in_provider_name = this->edit_provider_name_->text().toStdString();
    std::string in_platform_name = this->edit_platform_name_->text().toStdString();
    std::string in_url = this->edit_url_->text().toStdString();
    std::string in_category_name = this->combo_category_->currentText().toStdString();
    std::string in_hotline = this->edit_hotline_->text().toStdString();
    std::string in_username = this->edit_username_->text().toStdString();
    std::string in_nickname = this->edit_nickname_->text().toStdString();
    std::string in_password = this->edit_password_->text().toStdString();
    std::string in_sub_account = this->edit_sub_account_->text().toStdString();
    std::string in_phone = this->combo_phone_->currentText().toStdString();
    std::string in_email = this->combo_email_->currentText().toStdString();
    std::string in_postscript = this->edit_postscript_->toPlainText().toStdString();

    std::string in_main_key = this->edit_main_key_->text().toStdString();

    // 2. 数据合法性验证

    // 3. 新增记录
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
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
