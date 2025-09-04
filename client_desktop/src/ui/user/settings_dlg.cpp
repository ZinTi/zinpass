#include "user/settings_dlg.h"

#include <QMessageBox>

SettingsDlg::SettingsDlg(QWidget* parent) : QDialog(parent) {
    setup_ui();
}

SettingsDlg::~SettingsDlg() {

}

void SettingsDlg::setup_ui() {
    this->setWindowTitle("设置 | ZinPass");
    this->setMaximumSize(480, 300);

    e_display_ = new QTextEdit(this);
    e_display_->setReadOnly(true);
    // e_display_->setPlaceholderText("settings");
    e_display_->setMaximumHeight(100);

    l_theme_ = new QLabel("主题", this);
    l_language_ = new QLabel("语言", this);
    l_ip_address_ = new QLabel("IP地址", this);
    l_port_ = new QLabel("端口", this);
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

    e_ip_address_ = new QLineEdit(this);
    e_ip_address_->setPlaceholderText("127.0.0.1");
    e_ip_address_->setText("127.0.0.1");
    e_port_ = new QLineEdit(this);
    e_port_->setValidator(new QIntValidator(1, 65535, this));
    e_port_->setPlaceholderText("0-65535");
    e_port_->setText("50051");
    e_port_->setMaximumWidth(80);

    bg_theme_ = new QButtonGroup(this);
    btn_dark_mode_ = new QRadioButton("深色", this); // 深色主题开关
    btn_light_mode_ = new QRadioButton("浅色", this); // 浅色主题开关
    btn_system_mode_ = new QRadioButton("跟随系统", this); // 跟随系统主题开关
    btn_system_mode_->setChecked(true);
    bg_theme_->addButton(btn_dark_mode_);
    bg_theme_->addButton(btn_light_mode_);
    bg_theme_->addButton(btn_system_mode_);

    combo_language_ = new QComboBox(this); // 语言选择
    combo_language_->addItem("简体中文");
    combo_language_->addItem("繁体中文");
    combo_language_->addItem("English(USA)");
    combo_language_->addItem("English(UK)");
    combo_language_->addItem("Français");
    combo_language_->addItem("Español");       // 西班牙语
    combo_language_->addItem("Русский");       // 俄语
    combo_language_->addItem("한국어");         // 韩国语
    combo_language_->addItem("日本語");
    combo_language_->addItem("Deutsch");       // 德语

    btn_ping_ = new QPushButton("Ping", this);     // ping 服务器测试网络连接
    btn_reset_ = new QPushButton("重置", this);
    btn_refresh_ = new QPushButton("刷新", this);
    btn_apply_ = new QPushButton("应用", this);
    btn_save_ = new QPushButton("保存",this);     // 保存设置

    lyt_main_ = new QVBoxLayout(this);
    lyt_grid_ = new QGridLayout(this);
    lyt_btn_ = new QHBoxLayout(this);

    lyt_grid_->addWidget(l_theme_, 0, 0);
    lyt_grid_->addWidget(btn_dark_mode_, 0, 1);
    lyt_grid_->addWidget(btn_light_mode_, 0, 2);
    lyt_grid_->addWidget(btn_system_mode_, 0, 3);
    lyt_grid_->addWidget(l_language_, 0, 4);
    lyt_grid_->addWidget(combo_language_, 0, 5, 1, 2);

    lyt_grid_->addWidget(l_ip_address_, 1,0); // 行 列
    lyt_grid_->addWidget(e_ip_address_, 1,1, 1, 3); // 行 列 行跨度 列跨度
    lyt_grid_->addWidget(l_port_, 1, 4);
    lyt_grid_->addWidget(e_port_, 1, 5);
    lyt_grid_->addWidget(btn_ping_, 1, 6);

    lyt_btn_->addStretch();
    lyt_btn_->addWidget(btn_reset_);
    lyt_btn_->addWidget(btn_refresh_);
    lyt_btn_->addWidget(btn_apply_);
    lyt_btn_->addWidget(btn_save_);
    lyt_btn_->addStretch();

    lyt_main_->addWidget(e_display_);
    lyt_main_->addLayout(lyt_grid_);
    lyt_main_->addLayout(lyt_btn_);

    connect(btn_ping_, &QPushButton::clicked, this, &SettingsDlg::on_btn_ping_clicked);
    connect(btn_reset_, &QPushButton::clicked, this, &SettingsDlg::on_btn_reset_clicked);
    connect(btn_refresh_, &QPushButton::clicked, this, &SettingsDlg::on_btn_refresh_clicked);
    connect(btn_apply_, &QPushButton::clicked, this, &SettingsDlg::on_btn_apply_clicked);
    connect(btn_save_, &QPushButton::clicked, this, &SettingsDlg::on_btn_save_clicked);
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
