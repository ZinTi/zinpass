#include "popup_filter_window.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>


PopupFilterWindow::PopupFilterWindow(QWidget* parent) : QWidget(parent, Qt::Popup) {
    this->lyt_main_ = new QVBoxLayout(this);
    this->l_title_ = new QLabel(this);
    this->l_title_->setText(QString("更多筛选条件"));
    this->l_title_->setStyleSheet("color: yellow; font-size: 10pt;");
    this->l_title_->setAlignment(Qt::AlignCenter);

    this->lyt_btn_ = new QHBoxLayout(this);
    this->btn_reset_ = new QPushButton("清空", this); // 清空筛选条件的Btn
    this->btn_reset_->setMaximumWidth(120); // 设置最大宽度
    this->lyt_btn_->addWidget(this->btn_reset_, 0, Qt::AlignHCenter);

    this->lyt_main_->addWidget(this->l_title_); // 添加 title

    // 创建筛选控件
    this->l_url_ = new QLabel("网址", this);
    this->e_url_ = new QLineEdit(this);
    this->e_url_->setEnabled(false);

    this->l_hotline_ = new QLabel("客服热线", this);
    this->e_hotline_ = new QLineEdit(this);
    this->e_hotline_->setEnabled(false);

    this->l_sub_acc_ = new QLabel("子账号", this);
    this->e_sub_acc_ = new QLineEdit(this);
    this->e_sub_acc_->setEnabled(false);

    this->l_cdt_range_ = new QLabel("创建时间范围", this);
    this->e_cdt_range_start_ = new QDateTimeEdit(this);
    this->e_cdt_range_start_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    this->e_cdt_range_start_->setCalendarPopup(true);
    this->e_cdt_range_start_->setEnabled(false);
    this->e_cdt_range_end_ = new QDateTimeEdit(this);
    this->e_cdt_range_end_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    this->e_cdt_range_end_->setCalendarPopup(true);
    this->e_cdt_range_end_->setEnabled(false);

    this->l_udt_range_ = new QLabel("修改时间范围", this);
    this->e_udt_range_start_ = new QDateTimeEdit(this);
    this->e_udt_range_start_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    this->e_udt_range_start_->setCalendarPopup(true);
    this->e_udt_range_start_->setEnabled(false);
    this->e_udt_range_end_ = new QDateTimeEdit(this);
    this->e_udt_range_end_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    this->e_udt_range_end_->setCalendarPopup(true);
    this->e_udt_range_end_->setEnabled(false);
    this->e_udt_range_end_->setDateTime(QDateTime::currentDateTime()); // 设置当前时间

    // 日期输入框布局
    this->lyt_cdt_range_ = new QVBoxLayout(this);
    this->lyt_cdt_range_->addWidget(this->e_cdt_range_start_);
    this->lyt_cdt_range_->addWidget(this->e_cdt_range_end_);
    this->lyt_udt_range_ = new QVBoxLayout(this);
    this->lyt_udt_range_->addWidget(this->e_udt_range_start_);
    this->lyt_udt_range_->addWidget(this->e_udt_range_end_);

    // 使用 QFormLayout 布局筛选控件
    this->lyt_filter_ = new QFormLayout(this);
    this->lyt_filter_->addRow(this->l_url_, this->e_url_);
    this->lyt_filter_->addRow(this->l_hotline_, this->e_hotline_);
    this->lyt_filter_->addRow(this->l_sub_acc_, this->e_sub_acc_);
    this->lyt_filter_->addRow(this->l_cdt_range_, this->lyt_cdt_range_);
    this->lyt_filter_->addRow(this->l_udt_range_, this->lyt_udt_range_);

    this->lyt_main_->addLayout(this->lyt_filter_); // 添加 this->lyt_filter_
    this->lyt_main_->addLayout(this->lyt_btn_); // 添加包含 btn_reset_ 的水平布局容器

    setLayout(this->lyt_main_);
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
