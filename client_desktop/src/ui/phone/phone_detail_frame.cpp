#include "phone/phone_detail_frame.h"

#include <QMessageBox>
#include "state/state_manager.h"
#include "state/channel_manager.h"
#include "rpc/phone_rpc.h"
#include "rpc/telecom_rpc.h"
#include "common/dlg_auth.h"
#include "common/toast.h"
#include "phone/deletion_guide_dlg.h"

PhoneDetailFrame::PhoneDetailFrame(QFrame *parent) : QFrame(parent){
    this->phone_ = new zinpass::models::MobilePhone;
    setup_ui();
}

PhoneDetailFrame::~PhoneDetailFrame(){
    if(this->phone_){
        delete this->phone_;
    }
}

void PhoneDetailFrame::list_telecom_operators() const {
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 获取 telecom_operators 数据
    zinpass::rpc::TelecomRPC telecom_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[telecoms, message_telecom] = telecom_rpc.list_telecom_options(session_id);

    // 3. 渲染到 UI 控件
    combo_telecom_->clear();
    for (const std::string& telecom : telecoms) {
        combo_telecom_->addItem(QString::fromStdString(telecom));
    }
}

void PhoneDetailFrame::setup_ui(){
    edit_id_ = new QLineEdit(this);
    edit_id_->setEnabled(false);
    combo_telecom_ = new QComboBox(this);
    edit_phone_area_ = new QLineEdit(this);
    edit_phone_ = new QLineEdit(this);
    edit_pin_ = new QLineEdit(this);
    edit_puk_ = new QLineEdit(this);
    edit_service_pwd_ = new QLineEdit(this);
    edit_join_time_ = new QDateTimeEdit(this);
    edit_join_time_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    edit_join_time_->setCalendarPopup(true); // 开启日历弹出功能
    edit_postscript_ = new QLineEdit(this);
    edit_user_id_ = new QLineEdit(this);
    edit_user_id_->setEnabled(false);
    edit_other_ = new QTextEdit(this);
    edit_other_->setPlaceholderText(
        "created_at: \n"
        "updated_at: \n");
    // edit_other_->setMaximumHeight(100);
    edit_other_->setReadOnly(true);

    btn_refresh_ = new QPushButton("刷新", this);
    btn_refresh_->setFixedWidth(60);
    btn_edit_ = new QPushButton("编辑", this);
    btn_edit_->setFixedWidth(60);
    btn_submit_ = new QPushButton("更新", this);
    btn_submit_->setFixedWidth(60);
    btn_delete_ = new QPushButton("删除", this);
    btn_delete_->setFixedWidth(60);

    lyt_main_ = new QHBoxLayout(this);
    lyt_form1_ = new QFormLayout(this);
    lyt_form2_ = new QFormLayout(this);
    lyt_form3_ = new QFormLayout(this);
    lyt_btn_ = new QVBoxLayout(this);
    lyt_main_->addLayout(lyt_form1_);
    lyt_main_->addLayout(lyt_form2_);
    lyt_main_->addLayout(lyt_form3_);
    lyt_main_->addLayout(lyt_btn_);

    lyt_form1_->addRow("ID", edit_id_);
    lyt_form1_->addRow("运营商", combo_telecom_);
    lyt_form1_->addRow("归属地", edit_phone_area_);
    lyt_form1_->addRow("手机号", edit_phone_);

    lyt_form2_->addRow("PIN", edit_pin_);
    lyt_form2_->addRow("PUK", edit_puk_);
    lyt_form2_->addRow("服务密码", edit_service_pwd_);
    lyt_form2_->addRow("入网时间", edit_join_time_);

    lyt_form3_->addRow("备注", edit_postscript_);
    lyt_form3_->addRow("所有者", edit_user_id_);
    lyt_form3_->addRow("其他信息", edit_other_); // id + created_at + updated_at + postscript + owner_id

    lyt_btn_->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    lyt_btn_->addWidget(btn_refresh_);
    lyt_btn_->addWidget(btn_edit_);
    lyt_btn_->addWidget(btn_submit_);
    lyt_btn_->addWidget(btn_delete_);

    initial_input_widgets();

    connect(btn_refresh_, &QPushButton::clicked, this, &PhoneDetailFrame::initial_input_widgets);
    connect(btn_edit_, &QPushButton::clicked, this, [this, b = false](){
        PhoneDetailFrame::set_input_read_only(b);   // 允许编辑输入框
        btn_edit_->setEnabled(false);         // 禁用“编辑”按钮
        btn_edit_->setText("编辑中");          // 变更“编辑”按钮文字
        btn_submit_->setEnabled(true);        // 启用“更新”按钮
    });
    connect(btn_submit_, &QPushButton::clicked, this, &PhoneDetailFrame::on_btn_submit_clicked);
    connect(btn_delete_, &QPushButton::clicked, this, &PhoneDetailFrame::on_btn_delete_clicked);
}

void PhoneDetailFrame::initial_input_widgets() const {
    edit_id_->clear();
    list_telecom_operators();   // 获取并列出电信运营商可选项
    edit_phone_area_->clear();
    edit_phone_->clear();
    edit_pin_->clear();
    edit_puk_->clear();
    edit_service_pwd_->clear();
    edit_join_time_->setDateTime(QDateTime::currentDateTime());
    edit_postscript_->clear();
    edit_other_->clear();

    set_input_read_only( true );            // 设置编辑框仅读
    btn_edit_->setEnabled( false );     // 默认“编辑” disable，获取数据后“编辑” enable
    btn_edit_->setText("编辑");
    btn_submit_->setEnabled( false );   // 默认“更新” disable，点击“编辑”后“更新” enable
    btn_delete_->setEnabled( false );   // 默认“删除” disable，填充数据后“删除” enable
}

void PhoneDetailFrame::set_input_read_only(const bool enable) const {
    combo_telecom_->setEnabled( !enable );
    edit_phone_area_->setReadOnly(enable);
    edit_phone_->setReadOnly(enable);

    edit_pin_->setReadOnly(enable);
    edit_puk_->setReadOnly(enable);
    edit_service_pwd_->setReadOnly(enable);
    edit_join_time_->setReadOnly(enable);
    edit_postscript_->setReadOnly(enable);
}

void PhoneDetailFrame::fetch_phone_by_id(const int id) const {
    // 1. 准备数据
    this->phone_->setId(id);    // 先设置id
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 查询数据
    zinpass::rpc::PhoneRPC phone_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[phone, message] = phone_rpc.find_phone_by_id(session_id, this->phone_->getId());
    this->phone_->copy(phone);
}

void PhoneDetailFrame::render_phone_to_ui() const {
    // enable "编辑"按钮 并禁用输入框编辑功能、暂时禁用编辑功能
    btn_edit_->setEnabled(true);
    btn_edit_->setText("编辑");
    set_input_read_only(true);
    if(btn_submit_->isEnabled()){
        btn_submit_->setEnabled(false);
    }
    btn_delete_->setEnabled(true);
    // 填充数据
    edit_id_->setText(QString("%1").arg(this->phone_->getId()));
    combo_telecom_->setCurrentText(QString::fromStdString(this->phone_->getTelecomOperator()));
    edit_phone_area_->setText(QString::fromStdString(this->phone_->getPhoneArea()));
    edit_phone_->setText(QString::fromStdString(this->phone_->getPhoneNumber()));
    edit_pin_->setText(QString::fromStdString(this->phone_->getPin()));
    edit_puk_->setText(QString::fromStdString(this->phone_->getPuk()));
    edit_service_pwd_->setText(QString::fromStdString(this->phone_->getServicePwd()));
    edit_postscript_->setText(QString::fromStdString(this->phone_->getPostscript()));
    edit_user_id_->setText(QString::fromStdString(this->phone_->getSysUserId()));

    QString other_info = QString(
        "[创建于] %1\n"
        "[更新于] %2\n"
        )
        .arg(this->phone_->getCreatedTime())
        .arg(this->phone_->getUpdatedTime());

    edit_other_->setText(other_info);

    if(this->phone_->getJoinTime().empty()){
        edit_join_time_->clear(); // 清除原值
        edit_join_time_->setDateTime(QDateTime());
        edit_join_time_->setSpecialValueText("空"); // 设置特殊值文本
        edit_join_time_->clearFocus(); // 让控件失去焦点，确保特殊值文本能正常显示
    }else{
        // 尝试将文本转换为 QDateTime 对象
        QDateTime join_time = QDateTime::fromString(
            QString::fromStdString(this->phone_->getJoinTime()),
            QString("yyyy-MM-dd HH:mm:ss"));
        if (join_time.isValid()) {  // 有效
            edit_join_time_->setDateTime(join_time);
        } else { // 转换失败
            edit_join_time_->clear();
            edit_join_time_->setDateTime(QDateTime());
            edit_join_time_->setSpecialValueText("无效值"); // 设置特殊值文本
            edit_join_time_->clearFocus(); // 让控件失去焦点，确保特殊值文本能正常显示
        }
    }
}

zinpass::models::MobilePhone PhoneDetailFrame::get_phone() const{
    return *this->phone_;
}

void PhoneDetailFrame::on_btn_submit_clicked(){
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 获取表单数据
    const QString in_phone_id = edit_id_->text();
    if(in_phone_id.isEmpty()){
        QMessageBox::warning(this, QString("无效数据"), QString::fromStdString("请先查询并选中一条数据后再操作"));
        return;
    }

    const QString in_telecom_operator = combo_telecom_->currentText();
    const QString in_phone_area = edit_phone_area_->text();
    const QString in_phone_number = edit_phone_->text();
    const QString in_pin = edit_pin_->text();
    const QString in_puk = edit_puk_->text();
    const QString in_service_password = edit_service_pwd_->text();
    const QString in_join_time = edit_join_time_->text();
    const QString in_postscript = edit_postscript_->text();

    // 更新
    zinpass::rpc::PhoneRPC phone_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const auto[result, message] = phone_rpc.update_phone_by_id(
        session_id,
        in_phone_id.toInt(),
        in_telecom_operator.toStdString(),
        in_phone_area.toStdString(),
        in_phone_number.toStdString(),
        in_pin.toStdString(),
        in_puk.toStdString(),
        in_service_password.toStdString(),
        in_join_time.toStdString(),
        in_postscript.toStdString()
        );
    if(result){
        QMessageBox::information(this, QString("完成"), QString("更新成功! %1").arg(message));
    }else{
        QMessageBox::warning(this, QString("失败"), QString("更新失败! %1").arg(message));
    }
}

void PhoneDetailFrame::on_btn_delete_clicked(){
    zinpass::rpc::PhoneRPC phone_rpc(zinpass::state::ChannelManager::get_instance().get_channel());
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const QString in_phone_id = edit_id_->text();
    if(in_phone_id.isEmpty()){
        QMessageBox::warning(this, QString("无效数据"), QString::fromStdString("请先查询并选中一条数据后再操作"));
        return;
    }
    const int dest_phone_id = in_phone_id.toInt();

    // 1. 检查子表引用数
    const auto ref_count = phone_rpc.get_reference_count(session_id, dest_phone_id);

    // 2. 第一次删除
    if(ref_count == 0) { // 直接删除
        const auto[result1, msg1, ref_count1] =
            phone_rpc.delete_phone_by_id(session_id, dest_phone_id, 0, -1);
        if(result1) emit sig_update_table(); // 通知父组件更新 table_view
        Toast::showToast(this, QString::fromStdString(msg1), 3000, QColor(0, 0, 0, 255), Qt::green);
        return; // 不管是否成功，都不再弹出二次删除框
    }

    // 3. 弹出确认框
    auto* deletion_guide_dlg = new DeletionGuideDlg(dest_phone_id, ref_count,this);
    if (const auto dlg_ret = deletion_guide_dlg->exec();
        dlg_ret == QDialog::Rejected || dlg_ret != QDialog::Accepted) {
        return;
    }
    const int selected_option = deletion_guide_dlg->get_selected_option();

    // 4. 第二次删除
    const auto[result2, msg2, ref_count2] =
    phone_rpc.delete_phone_by_id(session_id, dest_phone_id, selected_option, -1);

    // 5. 更新 UI
    if(result2) emit sig_update_table(); // 通知父组件更新 table_view
    Toast::showToast(this, QString::fromStdString(msg2), 3000, QColor(0, 0, 0, 255), Qt::green);

    delete deletion_guide_dlg;
}


