#include "settings_dlg.h"

#include <QMessageBox>

SettingsDlg::SettingsDlg(QWidget* parent) : QDialog(parent) {
    setup_ui();
}

SettingsDlg::~SettingsDlg() {

}

void SettingsDlg::setup_ui() {
    this->setWindowTitle("设置 | ZinPass");
    this->setMaximumSize(480, 300);

    this->e_display_ = new QTextEdit(this);
    this->e_display_->setReadOnly(true);
    // this->e_display_->setPlaceholderText("settings");
    this->e_display_->setMaximumHeight(100);

    this->l_theme_ = new QLabel("主题", this);
    this->l_language_ = new QLabel("语言", this);
    this->l_ip_address_ = new QLabel("IP地址", this);
    this->l_port_ = new QLabel("端口", this);
    QPalette palette_theme = l_theme_->palette(); // 获取当前调色板
    QPalette palette_language = l_language_->palette();
    QPalette palette_ip_address = l_ip_address_->palette();
    QPalette palette_port = l_port_->palette();
    palette_theme.setColor(QPalette::WindowText, QColor("#888888")); // 设置字体颜色（Qt::red 或 QColor("#FF0000")）
    palette_language.setColor(QPalette::WindowText, QColor("#888888"));
    palette_ip_address.setColor(QPalette::WindowText, QColor("#888888"));
    palette_port.setColor(QPalette::WindowText, QColor("#888888"));
    l_theme_->setPalette(palette_theme); // 应用调色板
    l_language_->setPalette(palette_theme);
    l_ip_address_->setPalette(palette_theme);
    l_port_->setPalette(palette_theme);

    this->e_ip_address_ = new QLineEdit(this);
    this->e_ip_address_->setPlaceholderText("127.0.0.1");
    this->e_ip_address_->setText("127.0.0.1");
    this->e_port_ = new QLineEdit(this);
    this->e_port_->setValidator(new QIntValidator(1, 65535, this));
    this->e_port_->setPlaceholderText("0-65535");
    this->e_port_->setText("50051");
    this->e_port_->setMaximumWidth(80);

    this->btn_group_theme_ = new QButtonGroup(this);
    this->btn_dark_mode_ = new QRadioButton("深色", this); // 深色主题开关
    this->btn_light_mode_ = new QRadioButton("浅色", this); // 浅色主题开关
    this->btn_system_mode_ = new QRadioButton("跟随系统", this); // 跟随系统主题开关
    this->btn_system_mode_->setChecked(true);
    this->btn_group_theme_->addButton(this->btn_dark_mode_);
    this->btn_group_theme_->addButton(this->btn_light_mode_);
    this->btn_group_theme_->addButton(this->btn_system_mode_);

    this->combo_language_ = new QComboBox(this); // 语言选择
    this->combo_language_->addItem("简体中文");
    this->combo_language_->addItem("繁体中文");
    this->combo_language_->addItem("English(USA)");
    this->combo_language_->addItem("English(UK)");
    this->combo_language_->addItem("Français");
    this->combo_language_->addItem("Español");       // 西班牙语
    this->combo_language_->addItem("Русский");       // 俄语
    this->combo_language_->addItem("한국어");         // 韩国语
    this->combo_language_->addItem("日本語");
    this->combo_language_->addItem("Deutsch");       // 德语

    this->btn_ping_ = new QPushButton("Ping", this);     // ping 服务器测试网络连接
    this->btn_reset_ = new QPushButton("重置", this);
    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_apply_ = new QPushButton("应用", this);
    this->btn_save_ = new QPushButton("保存",this);     // 保存设置

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_grid_ = new QGridLayout(this);
    this->lyt_btn_ = new QHBoxLayout(this);

    this->lyt_grid_->addWidget(this->l_theme_, 0, 0);
    this->lyt_grid_->addWidget(this->btn_dark_mode_, 0, 1);
    this->lyt_grid_->addWidget(this->btn_light_mode_, 0, 2);
    this->lyt_grid_->addWidget(this->btn_system_mode_, 0, 3);
    this->lyt_grid_->addWidget(this->l_language_, 0, 4);
    this->lyt_grid_->addWidget(this->combo_language_, 0, 5, 1, 2);

    this->lyt_grid_->addWidget(this->l_ip_address_, 1,0); // 行 列
    this->lyt_grid_->addWidget(this->e_ip_address_, 1,1, 1, 3); // 行 列 行跨度 列跨度
    this->lyt_grid_->addWidget(this->l_port_, 1, 4);
    this->lyt_grid_->addWidget(this->e_port_, 1, 5);
    this->lyt_grid_->addWidget(this->btn_ping_, 1, 6);

    this->lyt_btn_->addStretch();
    this->lyt_btn_->addWidget(this->btn_reset_);
    this->lyt_btn_->addWidget(this->btn_refresh_);
    this->lyt_btn_->addWidget(this->btn_apply_);
    this->lyt_btn_->addWidget(this->btn_save_);
    this->lyt_btn_->addStretch();

    this->lyt_main_->addWidget(this->e_display_);
    this->lyt_main_->addLayout(this->lyt_grid_);
    this->lyt_main_->addLayout(this->lyt_btn_);

    connect(this->btn_ping_, &QPushButton::clicked, this, &SettingsDlg::on_btn_ping_clicked);
    connect(this->btn_reset_, &QPushButton::clicked, this, &SettingsDlg::on_btn_reset_clicked);
    connect(this->btn_refresh_, &QPushButton::clicked, this, &SettingsDlg::on_btn_refresh_clicked);
    connect(this->btn_apply_, &QPushButton::clicked, this, &SettingsDlg::on_btn_apply_clicked);
    connect(this->btn_save_, &QPushButton::clicked, this, &SettingsDlg::on_btn_save_clicked);
}

void SettingsDlg::on_btn_ping_clicked() {
    QMessageBox::information(this, "Ping", "Pinging...");
}

void SettingsDlg::on_btn_reset_clicked() {
    QMessageBox::information(this, "重置", "Resetting...");
}

void SettingsDlg::on_btn_refresh_clicked() {
    QMessageBox::information(this, "刷新", "Refreshing...");
}

void SettingsDlg::on_btn_apply_clicked(){
    QMessageBox::information(this, "应用", "Applying...");
}

void SettingsDlg::on_btn_save_clicked() {
    QMessageBox::information(this, "保存", "Saving...");
}
