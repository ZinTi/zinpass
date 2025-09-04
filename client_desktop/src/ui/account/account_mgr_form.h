#ifndef ACCOUNT_MGR_FORM_H
#define ACCOUNT_MGR_FORM_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include "common/search_box.h"
#include "account_filter_form.h"
#include "dlg_add_account.h"
#include "dlg_edit_account.h"
// #include "popup_filter_window.h"

class AccountMgrForm final : public QWidget {
    Q_OBJECT

public:
    explicit AccountMgrForm(QWidget *parent = nullptr);
    ~AccountMgrForm() override;

private slots:
    void on_btn_read_passwd_clicked(); // 查询和显示明文密码
    void on_btn_add_account_clicked() const; // 增加密码记录
    void on_btn_detail_and_edit_clicked() const; // 更新记录
    void on_btn_remove_account_clicked();  // 删除密码记录
    void on_btn_tool2_clicked() const; // 工具占位
    void on_table_view_item_clicked(const QModelIndex &index);

    void list_accounts();    // 查询accounts

private:
    void setup_ui();

    SearchBox* search_box_;         // 搜索控件
    QTextEdit* coord_viewer_;       // 显示控件
    AccountFilterForm* filter_form_; // 筛选控件面板
    QStandardItemModel* table_model_;
    QTableView* table_view_;        // 表格
    DialogAddAccount* dlg_add_acc_; // 新增账号
    DialogEditAccount* dlg_edit_acc_; // 编辑账号

    QToolButton* btn_data_owner_;   // 数据归属
    QToolButton* btn_tool2_;
    QPushButton* btn_read_passwd_;  // 查看密码
    QPushButton* btn_add_account_;  // 新增
    QPushButton* btn_detail_and_edit_;  // 详情与编辑

    int row_of_table_view_ = 0; // 聚焦行
    int column_of_table_view_ = 0;  // 聚焦列

    // 布局
    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_display_and_filter_;
    QHBoxLayout* lyt_bottom_btn_;
};

#endif
