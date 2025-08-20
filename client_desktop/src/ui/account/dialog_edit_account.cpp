#include "account/dialog_edit_account.h"
#include "common/dialog_auth.h"
#include "state/state_manager.h"
#include "dialog_exposed_pwd.h"
#include <QMessageBox>
#include "dialog_delete_account.h"
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
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_grid_ = new QGridLayout(this); // 创建网格布局
    this->lyt_key_ = new QFormLayout(this); // 创建主密钥布局
    this->lyt_action_ = new QHBoxLayout(this);
    this->lyt_main_->addSpacing(30);
    this->lyt_main_->addLayout(this->lyt_grid_);
    this->lyt_main_->addLayout(this->lyt_action_);
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
    this->edit_password_->setPlaceholderText("密码、密钥或口令 (置空不修改)");
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

    // 主密钥输入框布局
    this->edit_main_key_ = new QLineEdit(this);
    this->edit_main_key_->setPlaceholderText("主密钥");
    this->edit_main_key_->setEnabled(false);
    this->edit_main_key_->setEchoMode(QLineEdit::Password);
    this->lyt_key_->addRow("主密钥", this->edit_main_key_);

    // 按钮布局
    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_read_passwd_ = new QPushButton("查看密码", this);
    this->btn_edit_ = new QPushButton("编辑", this);
    this->btn_submit_ = new QPushButton("提交", this);
    this->btn_delete_ = new QPushButton("删除", this);
    this->lyt_action_->addLayout(this->lyt_key_);
    this->lyt_action_->addWidget(this->btn_refresh_);
    this->lyt_action_->addWidget(this->btn_read_passwd_);
    this->lyt_action_->addWidget(this->btn_edit_);
    this->lyt_action_->addWidget(this->btn_submit_);
    this->lyt_action_->addWidget(this->btn_delete_);

    // 连接信号槽
    connect(this->btn_refresh_, &QPushButton::clicked, this, &DialogEditAccount::initial_input_widgets);
    connect(this->btn_read_passwd_, &QPushButton::clicked, this, &DialogEditAccount::on_btn_read_passwd_clicked);
    connect(this->btn_edit_, &QPushButton::clicked, this, [this, b = false](){
        DialogEditAccount::read_only(b);
    });
    // connect(this->btn_submit_, &QPushButton::clicked, this, &DialogEditAccount::form_submitted);
    connect(this->btn_submit_, &QPushButton::clicked, this, &DialogEditAccount::on_btn_submit_clicked);
    connect(this->btn_delete_, &QPushButton::clicked, this, &DialogEditAccount::on_btn_delete_clicked);

    // 当this->edit_password_不为空时启用this->edit_main_key_;
    connect(this->edit_password_, &QLineEdit::textChanged, this, &DialogEditAccount::update_edit_main_key_state);
}

void DialogEditAccount::initial_input_widgets() const {
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
    const auto[view_account, message_account] = account_rpc.list_account_by_id(session_id, this->account_id_);

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

    this->edit_provider_name_->setText(QString::fromStdString(view_account.getProviderName()));
    this->edit_platform_name_->setText(QString::fromStdString(view_account.getPlatformName()));
    this->edit_url_->setText(QString::fromStdString(view_account.getURL()));
    this->combo_category_->setCurrentText(QString::fromStdString(view_account.getCategory()));
    this->edit_hotline_->setText(QString::fromStdString(view_account.getHotline()));
    this->edit_username_->setText(QString::fromStdString(view_account.getUsername()));
    this->edit_nickname_->setText(QString::fromStdString(view_account.getNickname()));
    this->edit_password_->setStyleSheet(
        "QLineEdit { color: rgb(183, 110, 121); }"
        "QLineEdit:disabled { color: gray; }"
        );
    this->edit_sub_account_->setText(QString::fromStdString(view_account.getSubAccount()));
    this->combo_phone_->setCurrentText(QString::fromStdString(view_account.getPhone()));
    this->combo_email_->setCurrentText(QString::fromStdString(view_account.getEmail()));
    this->edit_postscript_->setPlainText(QString::fromStdString(view_account.getPostscript()));

    read_only(true); // 将所有控件设置为只读
}

void DialogEditAccount::read_only(const bool enable) const {
    this->btn_edit_->setEnabled( enable );
    this->btn_edit_->setText(enable ? "编辑" : "编辑中");
    this->btn_submit_->setEnabled( !enable );   // 默认提交按钮不可点击，点击编辑按钮后方可提交

    this->edit_provider_name_->setReadOnly(enable);
    this->edit_platform_name_->setReadOnly(enable);
    this->edit_url_->setReadOnly(enable);
    this->combo_category_->setEnabled( !enable );
    this->edit_hotline_->setReadOnly(enable);
    this->edit_username_->setReadOnly(enable);
    this->edit_nickname_->setReadOnly(enable);
    // if( !enable ) this->edit_password_->clear();
    this->edit_password_->setReadOnly(enable);
    this->edit_sub_account_->setReadOnly(enable);
    this->combo_phone_->setEnabled( !enable );
    this->combo_email_->setEnabled( !enable );
    this->edit_postscript_->setReadOnly(enable);
}

void DialogEditAccount::on_btn_read_passwd_clicked(){
    //this->edit_password_->setText("显示密码");
    DialogExposedPwd dialog_exposed_pwd(this->account_id_, this);
    dialog_exposed_pwd.setWindowTitle(QString("查看密码(%1)").arg(this->account_id_));
    dialog_exposed_pwd.exec();
}

void DialogEditAccount::on_btn_delete_clicked(){
    DialogDeleteAccount dialog_delete(this->account_id_, this);
    if(QDialog::Accepted == dialog_delete.exec()){
        qDebug() << "测试";
    }else{
        return;
    }
}

QMap<QString, QVariant> DialogEditAccount::get_form_data() const{
    return {
        {"provider_name", this->edit_provider_name_->text()},
        {"platform_name", this->edit_platform_name_->text()},
        {"url", this->edit_url_->text()},
        {"category", this->combo_category_->currentText()},
        {"hotline", this->edit_hotline_->text()},
        {"username", this->edit_username_->text()},
        {"nickname", this->edit_nickname_->text()},
        {"password", this->edit_password_->text()},
        {"sub_account", this->edit_sub_account_->text()},
        {"phone", this->combo_phone_->currentText()},
        {"email", this->combo_email_->currentText()},
        {"postscript", this->edit_postscript_->toPlainText()}
    };
}

void DialogEditAccount::on_btn_submit_clicked(){
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


    // 2. 验证身份并获取主密钥
    std::string password_key;
    /*
    if(false == in_password.empty()){ // 不更新密码时，不用验证身份
        DialogAuth dialog_auth(this);
        if(dialog_auth.exec() == QDialog::Accepted){
            password_key = dialog_auth.get_input_main_key().toStdString();
        }else{
            return;
        }
    }*/
    if(false == in_password.empty()){
        password_key = this->edit_main_key_->text().toStdString();
        if(password_key.empty()){
            QMessageBox::warning(this, QString("无法更新"), QString("你的操作需要更新账号的密码\n因此主密钥输入框不能为空！"));
            return;
        }
    }

    // 3. 更新记录
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
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
    const bool isEditPasswordEmpty = this->edit_password_->text().trimmed().isEmpty();

    if (isEditPasswordEmpty) {
        this->edit_main_key_->clear();
        this->edit_main_key_->setEnabled(false); // 强制禁用 this->edit_main_key_
    } else {
        this->edit_main_key_->setEnabled(true);  // 只要 this->edit_password_ 有内容就启用 this->edit_main_key_
    }
}
