#include "phone/dialog_add_phone.h"
#include <QMessageBox>
#include "state/state_manager.h"
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
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_form_ = new QFormLayout;
    this->lyt_btn_ = new QHBoxLayout;
    lyt_main_->addLayout(lyt_form_);
    lyt_main_->addLayout(lyt_btn_);

    this->edit_phone_ = new QLineEdit(this);
    this->edit_phone_->setPlaceholderText("输入手机号码");
    this->combo_tele_operator_ = new QComboBox(this);
    this->edit_service_password_ = new QLineEdit(this);
    this->edit_service_password_->setPlaceholderText("办理业务时的服务密码");
    this->edit_pin_ = new QLineEdit(this);
    this->edit_pin_->setPlaceholderText("SIM卡-个人识别码");
    this->edit_puk_ = new QLineEdit(this);
    this->edit_puk_->setPlaceholderText("SIM卡-PIN解锁密钥");
    this->edit_join_time_ = new QDateTimeEdit(this);
    this->edit_join_time_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    this->edit_join_time_->setCalendarPopup(true);
    this->edit_phone_area_ = new QLineEdit(this);
    this->edit_phone_area_->setPlaceholderText("手机号码归属地");
    this->edit_postscript_ = new QTextEdit(this);
    this->edit_postscript_->setPlaceholderText("其他信息");
    this->edit_postscript_->setMaximumHeight(64);
    this->edit_main_key_ = new QLineEdit(this);
    this->edit_main_key_->setPlaceholderText("主密钥");
    this->edit_main_key_->setEchoMode(QLineEdit::Password);

    lyt_form_->addRow("手机号码", this->edit_phone_);
    lyt_form_->addRow("运营商", this->combo_tele_operator_);
    lyt_form_->addRow("服务密码", this->edit_service_password_);
    lyt_form_->addRow("PIN", this->edit_pin_);
    lyt_form_->addRow("PUK", this->edit_puk_);
    lyt_form_->addRow("入网时间", this->edit_join_time_);
    lyt_form_->addRow("归属地", this->edit_phone_area_);
    lyt_form_->addRow("备注", this->edit_postscript_);
    lyt_form_->addRow("主密钥", this->edit_main_key_);

    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_clear_ = new QPushButton("清空", this);
    this->btn_submit_ = new QPushButton("提交", this);
    lyt_btn_->addWidget(this->btn_refresh_);
    lyt_btn_->addWidget(this->btn_clear_);
    lyt_btn_->addWidget(this->btn_submit_);
    lyt_btn_->setAlignment(Qt::AlignCenter);

    connect(this->btn_refresh_, &QPushButton::clicked, this, &DialogAddPhone::on_btn_refresh_clicked);
    connect(this->btn_clear_, &QPushButton::clicked, this, &DialogAddPhone::on_btn_clear_clicked);
    connect(this->btn_submit_, &QPushButton::clicked, this, &DialogAddPhone::on_btn_submit_clicked);
}

void DialogAddPhone::initial_edit_widget() const {
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::TelecomRPC telecom_rpc(channel);
    const auto[telecoms, message_telecom] = telecom_rpc.list_telecom_options(session_id);

    this->combo_tele_operator_->clear();
    for(const std::string& telecom : telecoms){
        this->combo_tele_operator_->addItem(QString::fromStdString(telecom));
    }
}

void DialogAddPhone::on_btn_refresh_clicked() const {
    this->edit_phone_->clear();
    this->edit_service_password_->clear();
    this->edit_pin_->clear();
    this->edit_puk_->clear();
    this->edit_phone_area_->clear();
    this->edit_postscript_->clear();
    this->edit_join_time_->setDateTime(QDateTime::currentDateTime());

    initial_edit_widget();
}

void DialogAddPhone::on_btn_clear_clicked() const {
    this->edit_phone_->clear();
    this->edit_service_password_->clear();
    this->edit_pin_->clear();
    this->edit_puk_->clear();
    this->edit_phone_area_->clear();
    this->edit_postscript_->clear();
    this->edit_join_time_->setDateTime(QDateTime::currentDateTime());

    this->combo_tele_operator_->setCurrentIndex(0);
}

void DialogAddPhone::on_btn_submit_clicked(){
    const QString in_phone_number = this->edit_phone_->text();
    const QString in_telecom_operator = this->combo_tele_operator_->currentText();
    const QString in_service_password = this->edit_service_password_->text();
    const QString in_pin = this->edit_pin_->text();
    const QString in_puk = this->edit_puk_->text();
    const QString in_join_time = this->edit_join_time_->text();
    const QString in_phone_area = this->edit_phone_area_->text();
    const QString in_postscript = this->edit_postscript_->placeholderText();

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);
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
