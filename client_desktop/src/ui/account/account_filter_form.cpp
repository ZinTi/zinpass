#include "account/account_filter_form.h"
#include "state/state_manager.h"
// RPC 封装
#include "rpc/account_rpc.h"
#include "rpc/phone_rpc.h"
#include "rpc/category_rpc.h"

AccountFilterForm::AccountFilterForm(QWidget *parent) : QWidget(parent){
    setup_ui();
    setup_layout();
    initial_input_widgets(); // 初始化输入控件
    initial_popup_window(); // 初始化下拉弹窗
}

AccountFilterForm::~AccountFilterForm(){
    if(this->popup_window_) { delete this->popup_window_; }
    if(this->box_filter_) { delete this->box_filter_; }
}


void AccountFilterForm::setup_ui(){
    // 创建控件
    this->box_filter_ = new QGroupBox("筛选条件", this);
    this->edit_provider_name_ = new QLineEdit(this);
    this->edit_platform_name_ = new QLineEdit(this);
    this->edit_username_ = new QLineEdit(this);
    this->edit_nickname_ = new QLineEdit(this);
    this->combo_phone_ = new QComboBox(this);
    this->combo_email_ = new QComboBox(this);
    this->combo_category_ = new QComboBox(this);
    this->edit_postscript_ = new QLineEdit(this);
    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_submit_ = new QPushButton("查询", this);
    this->btn_drop_down_more_ = new QToolButton(this);
    //this->btn_drop_down_more_->setText("更多条件");
    this->btn_drop_down_more_->setPopupMode(QToolButton::InstantPopup); // 设置弹出模式
    this->btn_drop_down_more_->setArrowType(Qt::DownArrow); // 显示下拉箭头
    this->btn_drop_down_more_->setMinimumWidth(60);

    // 设置占位符文本
    this->edit_provider_name_->setPlaceholderText("所属公司或组织");
    this->edit_platform_name_->setPlaceholderText("平台名称");
    this->edit_username_->setPlaceholderText("用户名或用户ID");
    this->edit_nickname_->setPlaceholderText("昵称或姓名");
    this->edit_postscript_->setPlaceholderText("备注");

    // 统一设置控件高度
    constexpr int fixed_height = 24;
    this->edit_provider_name_->setFixedHeight(fixed_height);
    this->edit_platform_name_->setFixedHeight(fixed_height);
    this->edit_username_->setFixedHeight(fixed_height);
    this->edit_nickname_->setFixedHeight(fixed_height);
    this->combo_phone_->setFixedHeight(fixed_height);
    this->combo_email_->setFixedHeight(fixed_height);
    this->combo_category_->setFixedHeight(fixed_height);
    this->edit_postscript_->setFixedHeight(fixed_height);
}

void AccountFilterForm::setup_layout(){
    // 主布局
    this->lyt_main_ = new QHBoxLayout(this);
    this->lyt_gbox_ = new QHBoxLayout(this);

    // 表单布局
    this->lyt_form_left_ = new QFormLayout(this);
    lyt_form_left_->addRow("服务商", this->edit_provider_name_);
    lyt_form_left_->addRow("平台名", this->edit_platform_name_);
    lyt_form_left_->addRow("用户名", this->edit_username_);
    lyt_form_left_->addRow("昵称", this->edit_nickname_);
    //lyt_form_left_->setSpacing(6);

    this->lyt_form_right_ = new QFormLayout(this);
    lyt_form_right_->addRow("电话", this->combo_phone_);
    lyt_form_right_->addRow("邮箱", this->combo_email_);
    lyt_form_right_->addRow("类别", this->combo_category_);
    lyt_form_right_->addRow("备注", this->edit_postscript_);
    //lyt_form_right_->setSpacing(6);

    // 按钮布局
    this->lyt_btn_ = new QVBoxLayout(this);
    lyt_btn_->addWidget(this->btn_refresh_);
    lyt_btn_->addWidget(this->btn_submit_);
    lyt_btn_->addWidget(this->btn_drop_down_more_);
    lyt_btn_->addStretch(); // 拉伸

    // 表单对齐方式
    lyt_form_left_->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);
    lyt_form_right_->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);
    // lyt_btn_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 组合布局
    lyt_gbox_->addLayout(lyt_form_left_);
    lyt_gbox_->addSpacing(8);
    lyt_gbox_->addLayout(lyt_form_right_);
    lyt_gbox_->addSpacing(8);
    lyt_gbox_->addLayout(lyt_btn_);
    // lyt_gbox_->setContentsMargins(20, 20, 20, 20);
    this->box_filter_->setLayout(lyt_gbox_);
    this->lyt_main_->addWidget(this->box_filter_);

    // 连接信号槽
    connect(this->btn_refresh_, &QPushButton::clicked, this, &AccountFilterForm::on_btn_refresh_clicked);
    connect(this->btn_submit_, &QPushButton::clicked, this, &AccountFilterForm::form_submitted);
}

void AccountFilterForm::initial_popup_window(){
    this->popup_window_ = new PopupFilterWindow(this);
    // this->popup_window->setStyleSheet("background-color: lightgreen; color: #333333;");

           // 2. 处理下拉按钮点击事件
    connect(this->btn_drop_down_more_, &QToolButton::clicked, [this]() {
        if (this->popup_window_->isHidden()) {
            QPoint pos = mapToGlobal(this->btn_drop_down_more_->pos());
            pos.setY(pos.y() + this->btn_drop_down_more_->height());
            this->popup_window_->move(pos); // 移动到下拉按键的下方
            this->popup_window_->show();
            // 给窗口设置了Qt::Popup属性后无法输入中文，强制更新焦点链
            this->popup_window_->activateWindow(); //强制激活popup窗口（焦点窗口）
        } else {
            this->popup_window_->hide();
        }
    });

           //QObject::connect(this->btn_drop_down_more, &QToolButton::clicked, this, &AccountMgrForm::on_btn_drop_down_more_clicked); // 处理下拉按钮点击事件

}

void AccountFilterForm::initial_input_widgets() const {
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
    this->combo_phone_->addItem(QString("所有")); // default
    this->combo_phone_->addItem(QString("空值(暂未实现)"));
    for(const std::string& phone_number : phone_numbers){
        this->combo_phone_->addItem(QString::fromStdString(phone_number));
    }

    this->combo_email_->clear();
    this->combo_email_->addItem(QString("所有"));
    this->combo_email_->addItem(QString("空值(暂未实现)"));
    for (const std::string& email_address : email_addresses) {
        this->combo_email_->addItem(QString::fromStdString(email_address));
    }

    this->combo_category_->clear();
    this->combo_category_->addItem(QString("所有"));
    this->combo_category_->addItem(QString("空值(暂未实现)"));
    for (const std::string& category : name_of_categories) {
        this->combo_category_->addItem(QString::fromStdString(category));
    }
}

void AccountFilterForm::on_btn_drop_down_more_clicked() const {
    if (this->popup_window_->isHidden()) {
        QPoint pos = mapToGlobal(this->btn_drop_down_more_->pos());
        pos.setY(pos.y() + this->btn_drop_down_more_->height());
        this->popup_window_->move(pos); // 移动到下拉按键的下方
        this->popup_window_->show();
        // 给窗口设置了Qt::Popup属性后无法输入中文，强制更新焦点链
        this->popup_window_->activateWindow(); //强制激活popup窗口（焦点窗口）
    } else {
        this->popup_window_->hide();
    }
}

void AccountFilterForm::on_btn_refresh_clicked() const {
    initial_input_widgets();     // 初始化控件
    this->edit_provider_name_->clear();
    this->edit_platform_name_->clear();
    this->edit_username_->clear();
    this->edit_nickname_->clear();
    this->edit_postscript_->clear();
}

QMap<QString, QVariant> AccountFilterForm::get_form_data() const{
    return {
        {"provider_name", this->edit_provider_name_->text()},
        {"platform_name", this->edit_platform_name_->text()},
        {"username", this->edit_username_->text()},
        {"nickname", this->edit_nickname_->text()},
        {"phone", this->combo_phone_->currentText()},
        {"email", this->combo_email_->currentText()},
        {"category", this->combo_category_->currentText()},
        {"postscript", this->edit_postscript_->text()}
    };
}
