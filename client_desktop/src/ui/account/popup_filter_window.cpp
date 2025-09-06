#include "account/popup_filter_window.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>


PopupFilterWindow::PopupFilterWindow(QWidget* parent) : QWidget(parent, Qt::Popup) {
    lyt_main_ = new QVBoxLayout(this);
    l_title_ = new QLabel(this);
    l_title_->setText(QString("更多筛选条件"));
    l_title_->setStyleSheet("color: yellow; font-size: 10pt;");
    l_title_->setAlignment(Qt::AlignCenter);

    lyt_btn_ = new QHBoxLayout(this);
    btn_reset_ = new QPushButton("清空", this); // 清空筛选条件的Btn
    btn_reset_->setMaximumWidth(120); // 设置最大宽度
    lyt_btn_->addWidget(btn_reset_, 0, Qt::AlignHCenter);

    lyt_main_->addWidget(l_title_); // 添加 title

    // 创建筛选控件
    l_url_ = new QLabel("网址", this);
    e_url_ = new QLineEdit(this);
    e_url_->setEnabled(false);

    l_hotline_ = new QLabel("客服热线", this);
    e_hotline_ = new QLineEdit(this);
    e_hotline_->setEnabled(false);

    l_sub_acc_ = new QLabel("子账号", this);
    e_sub_acc_ = new QLineEdit(this);
    e_sub_acc_->setEnabled(false);

    l_cdt_range_ = new QLabel("创建时间范围", this);
    e_cdt_range_start_ = new QDateTimeEdit(this);
    e_cdt_range_start_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    e_cdt_range_start_->setCalendarPopup(true);
    e_cdt_range_start_->setEnabled(false);
    e_cdt_range_end_ = new QDateTimeEdit(this);
    e_cdt_range_end_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    e_cdt_range_end_->setCalendarPopup(true);
    e_cdt_range_end_->setEnabled(false);

    l_udt_range_ = new QLabel("修改时间范围", this);
    e_udt_range_start_ = new QDateTimeEdit(this);
    e_udt_range_start_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    e_udt_range_start_->setCalendarPopup(true);
    e_udt_range_start_->setEnabled(false);
    e_udt_range_end_ = new QDateTimeEdit(this);
    e_udt_range_end_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    e_udt_range_end_->setCalendarPopup(true);
    e_udt_range_end_->setEnabled(false);
    e_udt_range_end_->setDateTime(QDateTime::currentDateTime()); // 设置当前时间

    // 日期输入框布局
    lyt_cdt_range_ = new QVBoxLayout(this);
    lyt_cdt_range_->addWidget(e_cdt_range_start_);
    lyt_cdt_range_->addWidget(e_cdt_range_end_);
    lyt_udt_range_ = new QVBoxLayout(this);
    lyt_udt_range_->addWidget(e_udt_range_start_);
    lyt_udt_range_->addWidget(e_udt_range_end_);

    // 使用 QFormLayout 布局筛选控件
    lyt_filter_ = new QFormLayout(this);
    lyt_filter_->addRow(l_url_, e_url_);
    lyt_filter_->addRow(l_hotline_, e_hotline_);
    lyt_filter_->addRow(l_sub_acc_, e_sub_acc_);
    lyt_filter_->addRow(l_cdt_range_, lyt_cdt_range_);
    lyt_filter_->addRow(l_udt_range_, lyt_udt_range_);

    lyt_main_->addLayout(lyt_filter_); // 添加 lyt_filter_
    lyt_main_->addLayout(lyt_btn_); // 添加包含 btn_reset_ 的水平布局容器

    setLayout(lyt_main_);
}

PopupFilterWindow::~PopupFilterWindow(){
}

bool PopupFilterWindow::event(QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        const QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (!geometry().contains(mouseEvent->globalPosition().toPoint())) {
            hide();
            return true;
        }
    }
    // 调用基类的事件处理方法，确保其他事件能正常处理
    return QWidget::event(event);
}
