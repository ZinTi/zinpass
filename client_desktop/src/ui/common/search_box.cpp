#include "common/search_box.h"
#include <QStyle>

SearchBox::SearchBox(QWidget *parent_)
    : QWidget(parent_)
{
    // 初始化布局
    lyt_ = new QHBoxLayout(this);
    lyt_->setContentsMargins(0, 0, 0, 0);

           // 创建输入框
    line_edit_ = new QLineEdit(this);
    line_edit_->setPlaceholderText("Search...");
    line_edit_->setClearButtonEnabled(true);

           // 创建搜索按钮
    search_button_ = new QPushButton(this);
    search_button_->setIcon(style()->standardIcon(QStyle::SP_FileDialogStart));
    search_button_->setToolTip("Search");

           // 添加部件到布局
    lyt_->addWidget(line_edit_);
    lyt_->addWidget(search_button_);

           // 连接信号槽
    connect(search_button_, &QPushButton::clicked,
            this, &SearchBox::on_search_button_clicked_);
    connect(line_edit_, &QLineEdit::returnPressed,
            this, &SearchBox::on_search_button_clicked_);
}

SearchBox::~SearchBox()
{
  // 自动释放子对象
}

void SearchBox::on_search_button_clicked_()
{
    if (const QString keyword_ = line_edit_->text().trimmed();
        !keyword_.isEmpty()) {
        emit search_triggered(keyword_);
    }
}
