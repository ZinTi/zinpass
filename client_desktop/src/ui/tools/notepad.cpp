#include "notepad.h"

Notepad::Notepad(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

Notepad::~Notepad() = default;

void Notepad::setup_ui() {
    // 主窗口设置
    this->setGeometry(0, 0, 988, 600);
    
    // 创建UI组件
    menu_bar_ = new QMenuBar(this);
    status_bar_ = new QStatusBar(this);
    
    editor1_ = new NumberedTextEdit(this);
    editor2_ = new NumberedTextEdit(this);
    
    btn_diff_ = new QPushButton("差异对比", this);
    btn_setting_ = new QPushButton("设置", this);
    
    // 设置菜单系统
    setup_menus();
    
    // 布局设置
    lyt_features_ = new QHBoxLayout();
    lyt_features_->addWidget(btn_diff_);
    lyt_features_->addWidget(btn_setting_);
    
    lyt_editors_ = new QHBoxLayout();
    lyt_editors_->addWidget(editor1_);
    lyt_editors_->addWidget(editor2_);
    
    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(menu_bar_);      // 顶部菜单栏
    lyt_main_->addLayout(lyt_features_);  // 功能按钮区域
    lyt_main_->addLayout(lyt_editors_);   // 编辑器区域
    lyt_main_->addWidget(status_bar_);    // 底部状态栏
    
    // 初始状态栏消息
    status_bar_->showMessage("就绪");
}

void Notepad::setup_menus() {
    // 文件菜单
    QMenu* file_menu = menu_bar_->addMenu("文件");
    
    QAction* new_action = file_menu->addAction("新建");
    QAction* open_action = file_menu->addAction("打开");
    QAction* save_action = file_menu->addAction("保存");
    
    connect(new_action, &QAction::triggered, this, &Notepad::new_file);
    connect(open_action, &QAction::triggered, this, &Notepad::open_file);
    connect(save_action, &QAction::triggered, this, &Notepad::save_file);
}

// === 文件操作实现 ===
void Notepad::new_file() {
    status_bar_->showMessage("创建新文件", 3000);
    // 实际新建文件逻辑
}

void Notepad::open_file() {
    status_bar_->showMessage("打开文件", 3000);
    // 实际打开文件逻辑
}

void Notepad::save_file() {
    status_bar_->showMessage("保存文件", 3000);
    // 实际保存文件逻辑
}