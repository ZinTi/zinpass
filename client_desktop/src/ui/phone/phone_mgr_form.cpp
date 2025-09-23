#include "phone/phone_mgr_form.h"
#include "models/mobile_phone.h"
#include <QMessageBox>
#include "state/state_manager.h"
#include "state/channel_manager.h"
#include "rpc/phone_rpc.h"

PhoneMgrForm::PhoneMgrForm(QWidget *parent) : QWidget(parent){
    setup_ui();
    dlg_add_phone_ = new DialogAddPhone(this); // 初始化子对话框
}

PhoneMgrForm::~PhoneMgrForm(){
}

void PhoneMgrForm::setup_ui(){
    // 创建控件
    search_box_ = new SearchBox(this);
    this->display_ = new QTextEdit(this);
    this->display_->setMaximumHeight(120);
    if(false == this->display_->isReadOnly()){
        this->display_->setReadOnly(true);
    }
    filter_form_ = new PhoneFilterForm();
    filter_form_->setMaximumHeight(120);

    table_view_ = new QTableView;
    table_view_->setSortingEnabled(true);
    table_model_ = new QStandardItemModel(0, 12, this);
    table_model_->setHorizontalHeaderLabels({"ID","手机号","运营商","服务密码","PIN","PUK","入网时间","归属地","备注","所属用户","创建时间","更新时间"});
    table_view_->setModel(table_model_);

    this->phone_detail_frame_ = new PhoneDetailFrame;
    this->phone_detail_frame_->setMaximumHeight(135);

    btn_add_ = new QPushButton("新增", this);
    btn_clear_ = new QPushButton("清屏", this);

    // 创建布局
    lyt_main_ = new QVBoxLayout(this);
    lyt_top_ = new QHBoxLayout(this);
    lyt_btn_ = new QVBoxLayout(this);

    // 组合布局
    lyt_main_->addWidget(search_box_);
    lyt_main_->addLayout(lyt_top_);
    lyt_main_->addWidget(table_view_);
    lyt_main_->addWidget(this->phone_detail_frame_);

    lyt_top_->addWidget(this->display_);
    lyt_top_->addLayout(lyt_btn_);
    lyt_top_->addWidget(filter_form_);

    lyt_btn_->setAlignment(Qt::AlignCenter);
    lyt_btn_->addWidget(btn_add_);
    lyt_btn_->addWidget(btn_clear_);

    connect(search_box_, &SearchBox::search_triggered, this,
        [](const QString &keyword_){
            qDebug() << "用户在手机号管理界面点击了全局搜索操作\n";
        }
    );

    connect(filter_form_, &PhoneFilterForm::sig_form_submitted, this, &PhoneMgrForm::list_phones);
    connect(phone_detail_frame_, &PhoneDetailFrame::sig_update_table, this, &PhoneMgrForm::list_phones);
    connect(btn_add_, &QPushButton::clicked, this, &PhoneMgrForm::on_btn_add_clicked);
    connect(btn_clear_, &QPushButton::clicked, this, &PhoneMgrForm::on_btn_clear_clicked);
    connect(table_view_, &QTableView::clicked, this, &PhoneMgrForm::on_table_view_item_clicked); // 用户点击表格项，获取行列坐标保存到私有属性中，并显示Id列信息
}

void PhoneMgrForm::list_phones(){
    // 1. 准备数据
    auto data = filter_form_->get_form_data();
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    const std::string telecom_operator = (data["telecom"].toString().isEmpty() ? "" : data["telecom"].toString().toStdString());
    const std::string phone_number = (data["phone"].toString().isEmpty() ? "" : data["phone"].toString().toStdString());

    // 2. 如果输入数据合法则查询数据，否则结束
    zinpass::rpc::PhoneRPC phone_rpc(zinpass::state::ChannelManager::get_instance().get_channel());

    const auto[phones, message] = phone_rpc.list_phones(session_id, telecom_operator, phone_number);
    const unsigned int num_rows = static_cast<unsigned int>(phones.size());

    // 3. 渲染到 table view 中
    table_model_->setRowCount(num_rows);
    for(int i = 0 ; i < num_rows ; ++i){
        zinpass::models::MobilePhone row = phones[i];
        table_model_->setItem(i, 0, new QStandardItem(QString::number(row.getId())));
        table_model_->setItem(i, 1, new QStandardItem(QString::fromStdString(row.getPhoneNumber())));
        table_model_->setItem(i, 2, new QStandardItem(QString::fromStdString(row.getTelecomOperator())));
        table_model_->setItem(i, 3, new QStandardItem(QString::fromStdString(row.getServicePwd())));
        table_model_->setItem(i, 4, new QStandardItem(QString::fromStdString(row.getPin())));
        table_model_->setItem(i, 5, new QStandardItem(QString::fromStdString(row.getPuk())));
        table_model_->setItem(i, 6, new QStandardItem(QString::fromStdString(row.getJoinTime())));
        table_model_->setItem(i, 7, new QStandardItem(QString::fromStdString(row.getPhoneArea())));
        table_model_->setItem(i, 8, new QStandardItem(QString::fromStdString(row.getPostscript())));
        table_model_->setItem(i, 9, new QStandardItem(QString::fromStdString(row.getSysUserId())));
        table_model_->setItem(i, 10, new QStandardItem(QString::fromStdString(row.getCreatedTime())));
        table_model_->setItem(i, 11, new QStandardItem(QString::fromStdString(row.getUpdatedTime())));
    }

    // 4. 设置所有单元格为不可编辑
    for (int row = 0; row < table_model_->rowCount(); ++row) {
        for (int col = 0; col < table_model_->columnCount(); ++col) {
            QStandardItem *item = table_model_->item(row, col);
            if (item) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 移除可编辑标志
            }
        }
    }

    // 5、默认选中(0, 0)项
    if(0 >= table_model_->rowCount()){
        this->display_->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        this->display_->setText("没有找到符合的记录");
    }else{
        this->row_of_table_view_ = 0;
        this->column_of_table_view_ = 0;
        this->display_->setTextColor(QColor::fromRgbF(0, 100, 255, 1.0));
        QModelIndex index = table_model_->index(this->row_of_table_view_, 0);
        this->display_->setText(QString::fromStdString("查询到 %1 条手机号记录\n").arg(std::to_string(num_rows)));
        this->display_->append(QString("默认选中ID: %1").arg(table_model_->data(index).toString()));
    }
}

void PhoneMgrForm::on_btn_add_clicked() const {
    dlg_add_phone_->show();
}

void PhoneMgrForm::on_btn_clear_clicked() const {
    this->display_->clear();
}

void PhoneMgrForm::on_table_view_item_clicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    this->row_of_table_view_ = index.row();
    this->column_of_table_view_ = index.column();

    const QModelIndex index_of_first_column = index.sibling(this->row_of_table_view_, 0);  // 同一行，第0列
    const QString value_of_first_column = index.model()->data(index_of_first_column).toString();

    this->display_->setTextColor(QColor::fromRgbF(0, 255, 0, 1.0));
    this->display_->append(
        QString("[操作] 查看ID为 %1 的手机号详细信息").arg(value_of_first_column)
    );
    this->phone_detail_frame_->fetch_phone_by_id(value_of_first_column.toInt());
    this->phone_detail_frame_->render_phone_to_ui();
    this->display_->append(
        QString("[本地] 已填充至下方控件框中")
    );
}
