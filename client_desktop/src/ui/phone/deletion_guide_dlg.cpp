#include "deletion_guide_dlg.h"

#include <QButtonGroup>
#include <QMessageBox>
#include "state_manager.h"
#include "rpc/phone_rpc.h"

DeletionGuideDlg::DeletionGuideDlg(const int phone_id, const int affected_records, QWidget *parent)
: QDialog(parent), phone_id_(phone_id), affected_records_(affected_records) {
    setup_ui();
    initial_combo_box();
}

DeletionGuideDlg::~DeletionGuideDlg() {}

int DeletionGuideDlg::get_selected_option() const {
    return this->selected_option_;
}

void DeletionGuideDlg::setup_ui() {
    this->setWindowTitle("删除引导");
    this->setMaximumSize(800, 300);
    this->setWhatsThis("This is a guide for deleting a contact or message.");

    this->l_title_ = new QLabel("操作受阻，拒绝服务", this);

    this->txt_guide_ = new QTextEdit(this);
    this->txt_guide_->setReadOnly(true);
    this->txt_guide_->setMaximumSize(800, 80);
    this->txt_guide_->setHtml(
        QString("<p>前置条件不满足，无法直接完成删除操作</p>"
        "<p>目标手机号 %1 被 %2 条账号引用，需要先处理受影响数据，请选择以下选项：</p>").arg(this->phone_id_).arg(this->affected_records_)
        );

    this->combo_phone_ = new QComboBox(this);

    this->group_rdo_btn_ = new QButtonGroup(this);
    this->group_rdo_btn_->setExclusive(true); // 互斥
    this->rdo_option1_ = new QRadioButton("【推荐】放弃删除操作，稍后手动为手机号添加备注为“停用”", this);
    this->rdo_option2_ = new QRadioButton("【允许】批量修改受影响数据的“手机号”一项，改为", this);
    this->rdo_option3_ = new QRadioButton("【默认】擦除所有受影响数据的“手机号”一项（设置为NULL），然后完成删除操作", this);
    this->rdo_option4_ = new QRadioButton("【危险】级联删除所有受影响数据的记录（所有引用了该手机号的账号数据都将被删除）", this);

    this->group_rdo_btn_->addButton(this->rdo_option1_, 0);
    this->group_rdo_btn_->addButton(this->rdo_option2_, 1);
    this->group_rdo_btn_->addButton(this->rdo_option3_, 2);
    this->group_rdo_btn_->addButton(this->rdo_option4_, 3);
    this->rdo_option1_->setChecked(true);

    this->dlg_btn_box_ = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, // 添加标准按钮
        this
        );

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_title_ = new QHBoxLayout(this);
    this->lyt_rdo_ = new QVBoxLayout(this);
    this->lyt_rdo_combo_ = new QHBoxLayout(this);
    this->group_box_ = new QGroupBox("选项", this);
    this->group_box_->setLayout(this->lyt_rdo_);

    this->lyt_title_->setAlignment(Qt::AlignCenter);
    this->lyt_title_->addWidget(this->l_title_);
    this->lyt_rdo_->addWidget(this->rdo_option1_);
    //this->lyt_rdo_->addWidget(this->rdo_option2_);
    this->lyt_rdo_combo_->addWidget(this->rdo_option2_);
    this->lyt_rdo_combo_->addWidget(this->combo_phone_);
    this->lyt_rdo_combo_->addWidget(new QLabel("，然后完成删除操作", this));
    this->lyt_rdo_->addLayout(this->lyt_rdo_combo_);
    this->lyt_rdo_->addWidget(this->rdo_option3_);
    this->lyt_rdo_->addWidget(this->rdo_option4_);
    this->lyt_main_->setAlignment(Qt::AlignCenter);
    this->lyt_main_->setContentsMargins(12, 12, 12, 12);
    this->lyt_main_->addLayout(this->lyt_title_);
    this->lyt_main_->addWidget(this->txt_guide_);
    this->lyt_main_->addWidget(this->group_box_);
    this->lyt_main_->addWidget(this->dlg_btn_box_);

    connect(this->dlg_btn_box_, &QDialogButtonBox::accepted, this, &DeletionGuideDlg::on_btn_ok_clicked);
    connect(this->dlg_btn_box_, &QDialogButtonBox::rejected, this, [=](){ this->reject(); }); // 关闭对话框并返回 QDialog::Rejected
}

void DeletionGuideDlg::initial_combo_box() const {
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 创建gRPC通道连接到服务器获取数据：可选 telecom_operators、phone_numbers
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[phone_numbers, message_phone] = phone_rpc.list_phone_numbers(session_id);

    // 3. 渲染到 UI 控件
    this->combo_phone_->clear();
    for(const std::string& phone_number : phone_numbers){
        this->combo_phone_->addItem(QString::fromStdString(phone_number));
    }
}

void DeletionGuideDlg::on_btn_ok_clicked(){
    this->selected_option_ = this->group_rdo_btn_->checkedId(); // 获取选中值
    this->accept();
}

