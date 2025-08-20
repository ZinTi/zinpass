#include "account/account_mgr_form.h"
#include <QMessageBox>

#include "dialog_exposed_pwd.h"
#include "common/is_input_valid.h"
#include "common/dialog_auth.h"
#include "state/state_manager.h"
#include "models/view_account.h"

// RPC 封装
#include "rpc/account_rpc.h"
#include "common/toast.h"

// #include "pwd_generator_dlg.h"

AccountMgrForm::AccountMgrForm(QWidget *parent) : QWidget(parent){
    setup_ui();

}

AccountMgrForm::~AccountMgrForm(){
    if(this->table_model_){
        delete this->table_model_;
    }
    if(this->dlg_add_acc_){
        delete this->dlg_add_acc_;
    }
}

void AccountMgrForm::setup_ui(){
    // 创建控件
    this->search_box_ = new SearchBox(this);
    this->coord_viewer_ = new QTextEdit(this);
    this->coord_viewer_->setMaximumHeight(170);
    if(false == this->coord_viewer_->isReadOnly()) this->coord_viewer_->setReadOnly(true);
    this->filter_form_ = new AccountFilterForm(this);
    this->filter_form_->setMaximumHeight(180);

    this->table_view_ = new QTableView(this);
    this->table_view_->setSortingEnabled(true); // 启用表头点击排序
    this->table_model_ = new QStandardItemModel(0, 14, this);
    this->table_model_->setHorizontalHeaderLabels({"ID", "服务商", "平台名", "用户名（账号）", "昵称", "子账号", "绑定手机号", "绑定邮箱", "备注", "网址", "客服热线", "类别", "创建时间", "更新时间"});
    this->table_view_->setModel(this->table_model_); // 关联 model_ 和 table_view_

    this->btn_data_owner_ = new QToolButton(this);
    this->btn_data_owner_->setText("数据归属");
    this->btn_tool2_ = new QToolButton(this); // 工具占位按键
    this->btn_tool2_->setText("工具占位");
    this->btn_read_passwd_ = new QPushButton("查看密码", this);  // 查看密码
    this->btn_add_account_ = new QPushButton("新增", this);  // 新增
    this->btn_detail_and_edit_ = new QPushButton("详情与编辑", this);  // 编辑（更新/删除）

    this->dlg_add_acc_ = new DialogAddAccount(this);
    this->dlg_add_acc_->setWindowTitle(QString("新增账号密码"));
    this->dlg_add_acc_->setContentsMargins(12,10,12,10);
    //QModelIndex index = this->table_model_->index(this->row_of_table_view_, 0);
    //const std::string& dest_account_id = this->table_model_->data(index).toLongLong();

    // 创建布局
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_display_and_filter_ = new QHBoxLayout(this);
    this->lyt_bottom_btn_ = new QHBoxLayout(this);

    // 组合布局
    lyt_display_and_filter_->addWidget(this->coord_viewer_);
    lyt_display_and_filter_->addWidget(this->filter_form_);

    lyt_bottom_btn_->addWidget(this->btn_data_owner_);
    // lyt_bottom_btn_->addSpacing(30);
    lyt_bottom_btn_->addStretch();
    lyt_bottom_btn_->addWidget(this->btn_read_passwd_);
    lyt_bottom_btn_->addWidget(this->btn_add_account_);
    lyt_bottom_btn_->addWidget(this->btn_detail_and_edit_);
    // lyt_bottom_btn_->addSpacing(30);
    lyt_bottom_btn_->addStretch();
    lyt_bottom_btn_->addWidget(this->btn_tool2_);

    lyt_main_->addWidget(this->search_box_);
    lyt_main_->addLayout(lyt_display_and_filter_);
    lyt_main_->addWidget(this->table_view_);
    lyt_main_->addLayout(lyt_bottom_btn_);

    // 连接信号槽
    connect(this->search_box_, &SearchBox::search_triggered, this,
        [](const QString &keyword_){
            qDebug() << "用户在账号密码管理界面点击了全局搜索操作\n";
        }
    );

    QObject::connect(this->filter_form_, &AccountFilterForm::form_submitted,
        [&](){
            list_accounts();
        }
    );

    connect(this->btn_data_owner_, &QToolButton::clicked, this, [](){ qDebug() << "点击了 数据所属用户\n";});
    connect(this->btn_tool2_, &QToolButton::clicked, this, &AccountMgrForm::on_btn_tool2_clicked);
    connect(this->btn_read_passwd_, &QPushButton::clicked, this, &AccountMgrForm::on_btn_read_passwd_clicked);
    connect(this->btn_add_account_, &QPushButton::clicked, this, &AccountMgrForm::on_btn_add_account_clicked);
    connect(this->btn_detail_and_edit_, &QPushButton::clicked, this, &AccountMgrForm::on_btn_detail_and_edit_clicked);
    connect(this->table_view_, &QTableView::clicked, this, &AccountMgrForm::on_table_view_item_clicked);
}

void AccountMgrForm::list_accounts(){
    auto data = this->filter_form_->get_form_data();
    /*
    qDebug() << "提交数据:"
             << "\n(1)服务商:" << data["provider_name"].toString()
             << "\n(2)平台名:" << data["platform_name"].toString()
             << "\n(3)用户名:" << data["username"].toString()
             << "\n(4)昵称:" << data["nickname"].toString()
             << "\n(5)电话:" << data["phone"].toString()
             << "\n(6)邮箱:" << data["email"].toString()
             << "\n(7)类别:" << data["category"].toString()
             << "\n(8)备注:" << data["postscript"].toString();*/

    // 1. 声明变量、检查数据合法性
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    std::string provider_name = (data["provider_name"].toString().isEmpty() ? "" : data["provider_name"].toString().toStdString());
    std::string platform_name = (data["platform_name"].toString().isEmpty() ? "" : data["platform_name"].toString().toStdString());
    std::string username = (data["username"].toString().isEmpty() ? "" : data["username"].toString().toStdString());
    std::string nickname = (data["nickname"].toString().isEmpty() ? "" : data["nickname"].toString().toStdString());
    std::string mobile_phone = (data["phone"].toString() == "所有" ? "" : data["phone"].toString().toStdString()); // 所有、空值、数据库 mobile phone 可选项
    std::string email = (data["email"].toString() == "所有" ? "" : data["email"].toString().toStdString()); // 所有、空值、数据库 email 可选项
    std::string category = (data["category"].toString() == "所有" ? "" : data["category"].toString().toStdString());
    std::string postscript = (data["postscript"].toString().isEmpty() ? "" : data["postscript"].toString().toStdString());

    std::string sub_account = "";
    std::string url = "";
    std::string hotline = "";

    // 2. 如果输入数据合法则查询数据，否则结束
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);

    const auto[accounts, message] = account_rpc.list_accounts(
        session_id,
        provider_name,
        platform_name,
        username,
        nickname,
        sub_account,
        mobile_phone,
        email,
        postscript,
        url,
        hotline,
        category,
        10
        );
    unsigned int num_rows = static_cast<unsigned int>(accounts.size());

    // 3. 渲染到 table view 中
    this->table_model_->setRowCount(num_rows);
    for(int i = 0 ; i < num_rows ; ++i){
        zinpass::models::ViewAccount row = accounts[i];
        this->table_model_->setItem(i, 0, new QStandardItem(QString::fromStdString(row.getId())));
        this->table_model_->setItem(i, 1, new QStandardItem(QString::fromStdString(row.getProviderName())));
        this->table_model_->setItem(i, 2, new QStandardItem(QString::fromStdString(row.getPlatformName())));
        this->table_model_->setItem(i, 3, new QStandardItem(QString::fromStdString(row.getUsername())));
        this->table_model_->setItem(i, 4, new QStandardItem(QString::fromStdString(row.getNickname())));
        this->table_model_->setItem(i, 5, new QStandardItem(QString::fromStdString(row.getSubAccount())));
        this->table_model_->setItem(i, 6, new QStandardItem(QString::fromStdString(row.getPhone())));
        this->table_model_->setItem(i, 7, new QStandardItem(QString::fromStdString(row.getEmail())));
        this->table_model_->setItem(i, 8, new QStandardItem(QString::fromStdString(row.getPostscript())));
        this->table_model_->setItem(i, 9, new QStandardItem(QString::fromStdString(row.getURL())));
        this->table_model_->setItem(i, 10, new QStandardItem(QString::fromStdString(row.getHotline())));
        this->table_model_->setItem(i, 11, new QStandardItem(QString::fromStdString(row.getCategory())));
        this->table_model_->setItem(i, 12, new QStandardItem(QString::fromStdString(row.getCreatedTime())));
        this->table_model_->setItem(i, 13, new QStandardItem(QString::fromStdString(row.getUpdatedTime())));
    }

    // 4. 设置所有单元格为不可编辑
    for (int row = 0; row < this->table_model_->rowCount(); ++row) {
        for (int col = 0; col < this->table_model_->columnCount(); ++col) {
            QStandardItem *item = this->table_model_->item(row, col);
            if (item) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 移除可编辑标志
            }
        }
    }

    // 5、默认选中(0, 0)项
    if(0 >= this->table_model_->rowCount()){
        this->coord_viewer_->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        this->coord_viewer_->setText("没有找到符合的记录");
    }else{
        this->row_of_table_view_ = 0;
        this->column_of_table_view_ = 0;
        this->coord_viewer_->setTextColor(QColor::fromRgbF(0, 100, 255, 1.0));
        QModelIndex index = this->table_model_->index(this->row_of_table_view_, 0);
        this->coord_viewer_->setText(QString("默认选中ID: %1").arg(this->table_model_->data(index).toString()));
    }
}

void AccountMgrForm::on_btn_read_passwd_clicked() {
    const auto row_count = this->table_model_->rowCount();
    if (this->row_of_table_view_ < row_count) {
        const QStandardItem* item_column0 = this->table_model_->item(this->row_of_table_view_, 0);
        if (item_column0) {
            const QVariant data = item_column0->data(Qt::DisplayRole);
            const std::string account_id = data.toString().toStdString();

            DialogExposedPwd dialog_exposed_pwd(account_id, this);
            dialog_exposed_pwd.setWindowTitle(QString("查看密码(%1)").arg(account_id));
            dialog_exposed_pwd.exec();
        }
    } else {
        this->coord_viewer_->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        this->coord_viewer_->setText("表格中无数据！请先查询");
        return;
    }
}


void AccountMgrForm::on_btn_add_account_clicked() const {
    this->dlg_add_acc_->show();
}

void AccountMgrForm::on_btn_detail_and_edit_clicked() {
    if(this->row_of_table_view_ < this->table_model_->rowCount()){
        const QStandardItem* item_column0 = this->table_model_->item(this->row_of_table_view_, 0);
        if (item_column0) {
            const QVariant data = item_column0->data(Qt::DisplayRole);
            const std::string account_id = data.toString().toStdString();
            DialogEditAccount dlg_edit_acc(account_id, this);
            dlg_edit_acc.setWindowTitle(QString("详情与编辑(%1)").arg(account_id));
            dlg_edit_acc.setContentsMargins(12,10,12,10);
            dlg_edit_acc.exec();
        }
    } else {
        this->coord_viewer_->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        this->coord_viewer_->setText("表格中无数据！请先查询");
    }
}

void AccountMgrForm::on_btn_remove_account_clicked() {
    /*
    QString destId;
    const int rowCount = ui->tableWidgetAccount->rowCount();
    if (row_tableW < rowCount) {
        destId = ui->tableWidgetAccount->item(row_tableW, 0)->text();
    }
    else {
        ui->coordViewer->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        ui->coordViewer->setText("表格中无数据！请先查询");
        return;
    }
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; } // 二次鉴权
*/

    /*

    const auto accountMgr = new AccountManager();
    std::string msg;
    const bool result = accountMgr->deleteAccount(msg, destId.toInt());
    delete accountMgr;
    if(result){
        QMessageBox::information(this, QString::fromStdString("删除账号" + destId.toStdString() +"成功"), QString::fromStdString(msg));
    }else{
        QMessageBox::warning(this, QString::fromStdString("删除账号" + destId.toStdString() + "失败"), QString::fromStdString(msg));
    }
    // 删除用户后在ui->tableWidget中也删除那一行（相当于刷新操作，无需再次从数据库中查询）
    if(ui->tableWidgetAccount->item(row_tableW, 0)->text() == destId){ // 若还是这行（未因其他操作篡改）则直接删除
        ui->tableWidgetAccount->removeRow(row_tableW);
    }else{ // 否则找一下再删
        // bool found = false;
        for (int i = 0; i < rowCount; ++i) {
            if (ui->tableWidgetAccount->item(i, 0)->text() == destId) {
                ui->tableWidgetAccount->removeRow(i);
                // found = true;
                break;
            }
        }
    }*/
}

void AccountMgrForm::on_btn_tool2_clicked() const {
    Toast::showToast((QWidget*)this, "⚠️ Function not implemented yet!", 3000,QColor(200, 0, 200, 220), Qt::white);
}

void AccountMgrForm::on_table_view_item_clicked(const QModelIndex &index){
    if (!index.isValid()) return;

    this->row_of_table_view_ = index.row();
    this->column_of_table_view_ = index.column();

    const QModelIndex index_of_first_column = index.sibling(this->row_of_table_view_, 0);  // 同一行，第0列
    const QString value_of_first_column = index.model()->data(index_of_first_column).toString();

    this->coord_viewer_->setTextColor(QColor::fromRgbF(0, 255, 0, 1.0));
    this->coord_viewer_->setText(QString("已选中ID: %1").arg(value_of_first_column));
}

