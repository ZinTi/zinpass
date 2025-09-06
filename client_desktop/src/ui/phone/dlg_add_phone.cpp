#include "phone/dlg_add_phone.h"
#include <QMessageBox>
#include "state/state_manager.h"
#include "state/channel_manager.h"
#include "rpc/telecom_rpc.h"
#include "rpc/phone_rpc.h"

DialogAddPhone::DialogAddPhone(QWidget* parent) : QDialog(parent) {
    setup_ui();
    initial_edit_widget();  // 初始化控件
    setWindowTitle("新增手机号");
}

DialogAddPhone::~DialogAddPhone() {
}

void DialogAddPhone::setup_ui(){
    lyt_main_ = new QVBoxLayout(this);
    lyt_form_ = new QFormLayout;
    lyt_btn_ = new QHBoxLayout;
    lyt_main_->addLayout(lyt_form_);
    lyt_main_->addLayout(lyt_btn_);

    edit_phone_ = new QLineEdit(this);
    edit_phone_->setPlaceholderText("输入手机号码");
    combo_tele_operator_ = new QComboBox(this);
    edit_service_password_ = new QLineEdit(this);
    edit_service_password_->setPlaceholderText("办理业务时的服务密码");
    edit_pin_ = new QLineEdit(this);
    edit_pin_->setPlaceholderText("SIM卡-个人识别码");
    edit_puk_ = new QLineEdit(this);
    edit_puk_->setPlaceholderText("SIM卡-PIN解锁密钥");
    edit_join_time_ = new QDateTimeEdit(this);
    edit_join_time_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    edit_join_time_->setCalendarPopup(true);
    edit_phone_area_ = new QLineEdit(this);
    edit_phone_area_->setPlaceholderText("手机号码归属地");
    edit_postscript_ = new QTextEdit(this);
    edit_postscript_->setPlaceholderText("其他信息");
    edit_postscript_->setMaximumHeight(64);
    edit_main_key_ = new QLineEdit(this);
    edit_main_key_->setPlaceholderText("主密钥");
    edit_main_key_->setEchoMode(QLineEdit::Password);

    lyt_form_->addRow("手机号码", edit_phone_);
    lyt_form_->addRow("运营商", combo_tele_operator_);
    lyt_form_->addRow("服务密码", edit_service_password_);
    lyt_form_->addRow("PIN", edit_pin_);
    lyt_form_->addRow("PUK", edit_puk_);
    lyt_form_->addRow("入网时间", edit_join_time_);
    lyt_form_->addRow("归属地", edit_phone_area_);
    lyt_form_->addRow("备注", edit_postscript_);
    lyt_form_->addRow("主密钥", edit_main_key_);

    btn_refresh_ = new QPushButton("刷新", this);
    btn_clear_ = new QPushButton("清空", this);
    btn_submit_ = new QPushButton("提交", this);
    lyt_btn_->addWidget(btn_refresh_);
    lyt_btn_->addWidget(btn_clear_);
    lyt_btn_->addWidget(btn_submit_);
    lyt_btn_->setAlignment(Qt::AlignCenter);

    connect(btn_refresh_, &QPushButton::clicked, this, &DialogAddPhone::on_btn_refresh_clicked);
    connect(btn_clear_, &QPushButton::clicked, this, &DialogAddPhone::on_btn_clear_clicked);
    connect(btn_submit_, &QPushButton::clicked, this, &DialogAddPhone::on_btn_submit_clicked);
}

void DialogAddPhone::initial_edit_widget() const {
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    zinpass::rpc::TelecomRPC telecom_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[telecoms, message_telecom] = telecom_rpc.list_telecom_options(session_id);

    combo_tele_operator_->clear();
    for(const std::string& telecom : telecoms){
        combo_tele_operator_->addItem(QString::fromStdString(telecom));
    }
}

void DialogAddPhone::on_btn_refresh_clicked() const {
    edit_phone_->clear();
    edit_service_password_->clear();
    edit_pin_->clear();
    edit_puk_->clear();
    edit_phone_area_->clear();
    edit_postscript_->clear();
    edit_join_time_->setDateTime(QDateTime::currentDateTime());

    initial_edit_widget();
}

void DialogAddPhone::on_btn_clear_clicked() const {
    edit_phone_->clear();
    edit_service_password_->clear();
    edit_pin_->clear();
    edit_puk_->clear();
    edit_phone_area_->clear();
    edit_postscript_->clear();
    edit_join_time_->setDateTime(QDateTime::currentDateTime());

    combo_tele_operator_->setCurrentIndex(0);
}

void DialogAddPhone::on_btn_submit_clicked(){
    const QString in_phone_number = edit_phone_->text();
    const QString in_telecom_operator = combo_tele_operator_->currentText();
    const QString in_service_password = edit_service_password_->text();
    const QString in_pin = edit_pin_->text();
    const QString in_puk = edit_puk_->text();
    const QString in_join_time = edit_join_time_->text();
    const QString in_phone_area = edit_phone_area_->text();
    const QString in_postscript = edit_postscript_->placeholderText();

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    zinpass::rpc::PhoneRPC phone_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[result, message_phone] = phone_rpc.add_phone(
        session_id,
        in_telecom_operator.toStdString(),
        in_phone_area.toStdString(),
        in_phone_number.toStdString(),
        in_pin.toStdString(),
        in_puk.toStdString(),
        in_service_password.toStdString(),
        in_join_time.toStdString(),
        in_postscript.toStdString());

    if(result){
        QMessageBox::information(this,QString("成功"), QString("新增成功: %1").arg(message_phone));
    }else{
        QMessageBox::information(this,QString("失败"), QString("新增失败: %1").arg(message_phone));
    }
}
