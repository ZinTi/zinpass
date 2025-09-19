#include "phone/phone_filter_form.h"
#include "state/state_manager.h"
#include "state/channel_manager.h"
// RPC 封装
#include "rpc/telecom_rpc.h"
#include "rpc/phone_rpc.h"

PhoneFilterForm::PhoneFilterForm(QWidget *parent)
    : QWidget(parent){
    // setWindowTitle("筛选条件");
    setup_ui();
    initial_input_widgets(); // 初始化输入控件
}

PhoneFilterForm::~PhoneFilterForm(){
}


void PhoneFilterForm::setup_ui(){
    // 创建控件
    constexpr int fixed_height = 24;    // 统一高度
    constexpr int fixed_width = 56;
    box_filter_ = new QGroupBox("筛选条件", this);
    combo_telecom_ = new QComboBox(this);
    combo_telecom_->setFixedHeight(fixed_height);
    combo_telecom_->setMinimumWidth(120);
    combo_phone_ = new QComboBox(this);
    combo_phone_->setFixedHeight(fixed_height);
    combo_phone_->setMinimumWidth(120);
    btn_refresh_ = new QPushButton("刷新", this);
    btn_submit_ = new QPushButton("查询", this);
    btn_refresh_->setFixedSize(QSize(fixed_width,fixed_height)); // 宽 高
    btn_submit_->setFixedSize(QSize(fixed_width,fixed_height));

    // 主布局
    lyt_main_ = new QHBoxLayout(this);
    lyt_gbox_ = new QHBoxLayout(this);

    // 表单布局
    lyt_form_condition_ = new QFormLayout(this);
    lyt_form_condition_->addRow("运营商", combo_telecom_);
    lyt_form_condition_->addRow("手机号", combo_phone_);

    // 按钮布局
    lyt_btn_ = new QVBoxLayout(this);
    lyt_btn_->addWidget(btn_refresh_);
    lyt_btn_->addWidget(btn_submit_);
    lyt_btn_->addStretch(); // 拉伸

    // 表单对齐方式
    lyt_form_condition_->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 组合布局
    lyt_gbox_->addLayout(lyt_form_condition_);
    lyt_gbox_->addSpacing(8);
    lyt_gbox_->addLayout(lyt_btn_);
    // lyt_gbox_->setContentsMargins(20, 20, 20, 20);
    box_filter_->setLayout(lyt_gbox_);
    lyt_main_->addWidget(box_filter_);

    // 连接信号槽
    connect(btn_refresh_, &QPushButton::clicked, this, &PhoneFilterForm::on_btn_refresh_clicked);
    connect(btn_submit_, &QPushButton::clicked, this, &PhoneFilterForm::sig_form_submitted);
}

void PhoneFilterForm::initial_input_widgets() const {
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 获取数据：可选 telecom_operators、phone_numbers
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    zinpass::rpc::TelecomRPC telecom_rpc(channel);
    const auto[telecoms, message_telecom] = telecom_rpc.list_telecom_options(session_id);
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[phone_numbers, message_phone] = phone_rpc.list_phone_numbers(session_id);

    // 3. 渲染到 UI 控件
    combo_telecom_->clear();
    combo_telecom_->addItem(QString("所有"));
    for (const std::string& telecom : telecoms) {
        combo_telecom_->addItem(QString::fromStdString(telecom));
    }

    combo_phone_->clear();
    combo_phone_->addItem(QString("所有")); // default
    for(const std::string& phone_number : phone_numbers){
        combo_phone_->addItem(QString::fromStdString(phone_number));
    }
}

void PhoneFilterForm::on_btn_refresh_clicked() const {
    initial_input_widgets();     // 初始化控件
}

QMap<QString, QVariant> PhoneFilterForm::get_form_data() const{
    const QString telecom_operator = combo_telecom_->currentText();
    const QString phone_number = combo_phone_->currentText();
    return {
        {"telecom", (telecom_operator == "所有") ? "" : telecom_operator},
        {"phone", (phone_number == "所有") ? "" : phone_number}
    };
}
