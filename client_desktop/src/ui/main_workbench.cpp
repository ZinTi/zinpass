#include "main_workbench.h"
#include <QMessageBox>

MainWorkbench::MainWorkbench(QWidget* parent) : QMainWindow(parent) {
    setup_ui();

}

MainWorkbench::~MainWorkbench() {
    delete this->func_menu_form_; // 功能菜单（左侧）
    delete this->right_stacked_widget_; // 堆叠部件（右侧）

    /*// 这几个page添加到this->right_stacked_widget_以后由this->right_stacked_widget_统一管理和释放
    delete page_data_mgr_; // 数据管理-选项卡部件
    delete page_transfer_; // 迁移备份-选项卡部件
    delete page_log_; // 日志信息-选项卡部件
    delete page_setting_;

    // 这几个添加到page_data_mgr_以后由page_data_mgr_统一管理和释放
    delete acc_mgr_form_; // 管理账号
    delete phone_mgr_form_; // 管理手机号
    delete chk_pwd_security_form_; // 管理邮箱
    delete data_owner_form_; // 管理数据归属 */
}

void MainWorkbench::setup_ui() {
    // 1、设置窗口
    this->setGeometry(0, 0, 988, 600); // 设置窗口尺寸
    this->central_widget_ = new QWidget(this);
    this->setCentralWidget(this->central_widget_);

    // 2、创建菜单栏、顶级菜单和设置各个菜单栏
    this->menu_bar_ = new QMenuBar(this);
    this->menu_bar_->setStyleSheet(
        "background-color: rgb(100, 30, 100);"
        );
    QFont menu_font_ = this->menu_bar_->font();
    menu_font_.setPointSize(6);
    menu_bar_->setFont(menu_font_);
    this->setMenuBar(this->menu_bar_);
    create_menus();
    setup_menu_file();
    setup_menu_edit();
    setup_menu_view();
    setup_menu_setting();
    setup_menu_tools();
    setup_menu_help();

    // 3. 创建状态栏
    this->status_bar_ = new QStatusBar(this);

    // 4. 创建功能菜单（左侧）和堆叠部件（右侧）
    this->func_menu_form_ = new FunctionMenuForm(this);
    this->right_stacked_widget_ = new QStackedWidget(this);  // 堆叠部件
    this->lyt_main_ = new QHBoxLayout(this->central_widget_);
    this->lyt_main_->addWidget(this->func_menu_form_);
    this->lyt_main_->addWidget(this->right_stacked_widget_);

    page_data_mgr_ = new QTabWidget(this->right_stacked_widget_); // 数据管理-选项卡部件
    page_transfer_ = new QTabWidget(this->right_stacked_widget_); // 迁移备份-选项卡部件
    page_log_ = new QTabWidget(this->right_stacked_widget_); // 日志信息-选项卡部件
    page_setting_ = new QTabWidget(this->right_stacked_widget_); // 个人管理-选项卡部件

    // 添加page
    this->right_stacked_widget_->addWidget(page_data_mgr_);
    this->right_stacked_widget_->addWidget(page_transfer_);
    this->right_stacked_widget_->addWidget(page_log_);
    this->right_stacked_widget_->addWidget(page_setting_);

    page_data_mgr_init(); // 添加几个tab
    page_transfer_init();
    page_log_init();
    page_setting_init();

    // 设置拉伸因子，第一个参数是要设置的索引，第二个参数是拉伸因子
    // this->lyt_main_->setStretch(0, 1); // 给 this->func_menu_form_ 设置拉伸因子为 1
    this->lyt_main_->setStretch(1, 5); // 给 this->right_stacked_widget_ 设置拉伸因子为 5

    this->setStatusBar(this->status_bar_);

    connect(this->func_menu_form_, &FunctionMenuForm::sig_btn_clicked, this, &MainWorkbench::handle_menu_select);
}

void MainWorkbench::create_menus() {
    this->menu_file_ = this->menu_bar_->addMenu("文件(F)");
    this->menu_edit_ = this->menu_bar_->addMenu("编辑(E)");
    this->menu_view_ = this->menu_bar_->addMenu("查看(V)");
    this->menu_setting_ = this->menu_bar_->addMenu("设置(S)");
    this->menu_tools_ = this->menu_bar_->addMenu("工具(T)");
    this->menu_help_ = this->menu_bar_->addMenu("帮助(H)");
}

void MainWorkbench::setup_menu_file() {
    this->action_new_file_ = new QAction("新建(N)", this);
    this->action_new_file_->setShortcut(QKeySequence::New);
    connect(this->action_new_file_, &QAction::triggered, this, &MainWorkbench::onNew);

    this->action_open_file_ = new QAction("打开(O)", this);
    this->action_open_file_->setShortcut(QKeySequence::Open);
    connect(this->action_open_file_, &QAction::triggered, this, &MainWorkbench::onOpen);

    this->action_save_file_ = new QAction("保存(S)", this);
    this->action_save_file_->setShortcut(QKeySequence::Save);
    connect(this->action_save_file_, &QAction::triggered, this, &MainWorkbench::onSave);

    this->action_exit_ = new QAction("退出(X)", this);
    this->action_exit_->setShortcut(QKeySequence::Quit);
    connect(this->action_exit_, &QAction::triggered, qApp, &QApplication::quit);

    this->menu_file_->addAction(this->action_new_file_);
    this->menu_file_->addAction(this->action_open_file_);
    this->menu_file_->addAction(this->action_save_file_);
    this->menu_file_->addSeparator();
    this->menu_file_->addAction(this->action_exit_);
}
void MainWorkbench::setup_menu_edit() {
    this->action_copy_ = new QAction("复制(C)", this);
    this->action_copy_->setShortcut(QKeySequence::Copy);
    connect(this->action_copy_, &QAction::triggered, this, &MainWorkbench::onCopy);

    this->action_paste_ = new QAction("粘贴(P)", this);
    this->action_paste_->setShortcut(QKeySequence::Paste);
    connect(this->action_paste_, &QAction::triggered, this, &MainWorkbench::onPaste);

    this->action_cut_ = new QAction("剪切(T)", this);
    this->action_cut_->setShortcut(QKeySequence::Cut);
    connect(this->action_cut_, &QAction::triggered, this, &MainWorkbench::onCut);

    this->menu_edit_->addAction(this->action_copy_);
    this->menu_edit_->addAction(this->action_paste_);
    this->menu_edit_->addAction(this->action_cut_);
}
void MainWorkbench::setup_menu_view() {
    this->action_zoom_in_ = new QAction("放大(I)", this);
    this->action_zoom_in_->setShortcut(QKeySequence::ZoomIn);
    connect(this->action_zoom_in_, &QAction::triggered, this, &MainWorkbench::onZoomIn);

    this->action_zoom_out_ = new QAction("缩小(O)", this);
    this->action_zoom_out_->setShortcut(QKeySequence::ZoomOut);
    connect(this->action_zoom_out_, &QAction::triggered, this, &MainWorkbench::onZoomOut);

    this->action_reset_zoom_ = new QAction("重置(R)", this);
    this->action_reset_zoom_->setShortcut(tr("Ctrl+0"));
    connect(this->action_reset_zoom_, &QAction::triggered, this, &MainWorkbench::onResetZoom);

    this->menu_view_->addAction(this->action_zoom_in_);
    this->menu_view_->addAction(this->action_zoom_out_);
    this->menu_view_->addAction(this->action_reset_zoom_);
}
void MainWorkbench::setup_menu_setting() {
    this->action_options_ = new QAction("选项", this);
    connect(this->action_options_, &QAction::triggered, this, &MainWorkbench::onOptions);

    this->action_preferences_ = new QAction("首选项", this);
    connect(this->action_preferences_, &QAction::triggered, this, &MainWorkbench::onPreferences);

    this->action_customize_ = new QAction("个性化", this);
    connect(this->action_customize_, &QAction::triggered, this, &MainWorkbench::onCustomize);

    this->menu_setting_->addAction(this->action_options_);
    this->menu_setting_->addAction(this->action_preferences_);
    this->menu_setting_->addAction(this->action_customize_);
}
void MainWorkbench::setup_menu_tools() {
    this->action_passwd_gen_ = new QAction("密码生成器", this);
    connect(this->action_passwd_gen_, &QAction::triggered, this, &MainWorkbench::onPasswdGen);

    this->action_encrypt_ = new QAction("加密", this);
    connect(this->action_encrypt_, &QAction::triggered, this, &MainWorkbench::onEncrypt);

    this->action_decrypt_ = new QAction("解密", this);
    connect(this->action_decrypt_, &QAction::triggered, this, &MainWorkbench::onDecrypt);

    this->menu_tools_->addAction(this->action_passwd_gen_);
    this->menu_tools_->addAction(this->action_encrypt_);
    this->menu_tools_->addAction(this->action_decrypt_);
}
void MainWorkbench::setup_menu_help() {
    this->action_help_ = new QAction("帮助", this);
    this->action_help_->setShortcut(QKeySequence::HelpContents);
    connect(this->action_help_, &QAction::triggered, this, &MainWorkbench::onHelp);

    this->action_about_ = new QAction("关于", this);
    connect(this->action_about_, &QAction::triggered, this, &MainWorkbench::onAbout);

    this->action_update_ = new QAction("检查更新", this);
    connect(this->action_update_, &QAction::triggered, this, &MainWorkbench::onCheckUpdates);

    this->menu_help_->addAction(this->action_help_);
    this->menu_help_->addAction(this->action_update_);
    this->menu_help_->addSeparator();
    this->menu_help_->addAction(this->action_about_);
}

void MainWorkbench::handle_menu_select(const int index){
    // 设置当前显示的widget页面
    if(index>=1 && index<=4){
        this->right_stacked_widget_->setCurrentIndex(0);
    }else if(index>=5 && index<=6){
        this->right_stacked_widget_->setCurrentIndex(1);
    }else if(index>=7 && index<=8){
        this->right_stacked_widget_->setCurrentIndex(2);
    }else if(index>=9 && index<=11){
        this->right_stacked_widget_->setCurrentIndex(3);
    }else if (index==12) {
        // index==12 时不用更改
    } else {
        QMessageBox::information(this, QString("无效的按键信号"), QString::fromStdString(std::to_string(index)));
    }
    // 设置当前tab
    switch(index){
    case 1:{
        page_data_mgr_->setCurrentIndex(0);
        break;
    }
    case 2:{
        page_data_mgr_->setCurrentIndex(1);
        break;
    }
    case 3:{
        page_data_mgr_->setCurrentIndex(2);
        break;
    }
    case 4:{
        page_data_mgr_->setCurrentIndex(3);
        break;
    }
    case 5:{
        page_transfer_->setCurrentIndex(0);
        break;
    }
    case 6:{
        page_transfer_->setCurrentIndex(1);
        break;
    }
    case 7:{
        page_log_->setCurrentIndex(0);
        break;
    }
    case 8:{
        page_log_->setCurrentIndex(1);
        break;
    }
    case 9:{
        page_setting_->setCurrentIndex(0);
        break;
    }
    case 10:{
        page_setting_->setCurrentIndex(1);
        break;
    }
    case 11: {
        page_setting_->setCurrentIndex(2);
        break;
    }
    case 12:{
        close_this();
        // break;
    }
    default:
        ;
    }
}

// ================= 文件菜单槽函数实现 =================
void MainWorkbench::onNew() {
    qDebug() << "新建操作触发";
    QMessageBox::information(this, "新建", "创建新文件");
}

void MainWorkbench::onOpen() {
    qDebug() << "打开操作触发";
    QMessageBox::information(this, "打开", "打开文件对话框");
}

void MainWorkbench::onSave() {
    qDebug() << "保存操作触发";
    QMessageBox::information(this, "保存", "保存当前文件");
}

// ================= 编辑菜单槽函数实现 =================
void MainWorkbench::onCopy() {
    qDebug() << "复制操作触发";
}

void MainWorkbench::onPaste() {
    qDebug() << "粘贴操作触发";
}

void MainWorkbench::onCut() {
    qDebug() << "剪切操作触发";
}

// ================= 查看菜单槽函数实现 =================
void MainWorkbench::onZoomIn() {
    qDebug() << "放大视图";
}

void MainWorkbench::onZoomOut() {
    qDebug() << "缩小视图";
}

void MainWorkbench::onResetZoom() {
    qDebug() << "重置缩放比例";
}

// ================= 设置菜单槽函数实现 =================
void MainWorkbench::onOptions() {
    QMessageBox::information(this, "选项", "打开选项对话框");
}

void MainWorkbench::onPreferences() {
    QMessageBox::information(this, "首选项", "打开首选项面板");
}

void MainWorkbench::onCustomize() {
    QMessageBox::information(this, "自定义", "自定义工具栏设置");
}

// ================= 工具菜单槽函数实现 =================
void MainWorkbench::onPasswdGen() {
    auto* passwd_generator = new PwdGenDlg(this);
    passwd_generator->show();
}
void MainWorkbench::onEncrypt() {
    QMessageBox::information(this, "加密", "打开加密工具");
}
void MainWorkbench::onDecrypt() {
    QMessageBox::information(this, "解密", "打开解密工具");
}

// ================= 帮助菜单槽函数实现 =================
void MainWorkbench::onHelp() {
    QMessageBox::information(this, "帮助", "显示帮助文档");
}

void MainWorkbench::onAbout() {
    QMessageBox::about(this, "关于程序",
        "<b>ZinPass 守密司</b><br>"
        "版本 0.0.1<br><br>"
        "作者 曾来"
        );
}

void MainWorkbench::onCheckUpdates() {
    QMessageBox::information(this, "更新", "正在检查可用更新...");
}

void MainWorkbench::page_data_mgr_init(){
    acc_mgr_form_ = new AccountMgrForm(page_data_mgr_); // 账号维护
    page_data_mgr_->addTab(acc_mgr_form_, QString("账号维护"));
    phone_mgr_form_ = new PhoneMgrForm(page_data_mgr_);
    page_data_mgr_->addTab(phone_mgr_form_, QString("手机号码"));
    chk_pwd_security_form_ = new CheckPwdSecurityForm(page_data_mgr_);
    page_data_mgr_->addTab(chk_pwd_security_form_, QString("安全检查"));
    data_owner_form_ = new DataOwnerForm(page_data_mgr_);
    page_data_mgr_->addTab(data_owner_form_, QString("数据归属"));
}
void MainWorkbench::page_transfer_init(){
    output_form_ = new OutputForm(page_transfer_); // 账号维护
    page_transfer_->addTab(output_form_, QString("数据导出"));
    input_form_ = new InputForm(page_transfer_);
    page_transfer_->addTab(input_form_, QString("数据导入"));
}
void MainWorkbench::page_log_init(){
    log_run_form_ = new LogRunForm(page_log_);
    page_log_->addTab(log_run_form_, QString("运行日志"));
    log_handle_form_ = new LogHandleForm(page_log_);
    page_log_->addTab(log_handle_form_, QString("操作日志"));
}
void MainWorkbench::page_setting_init(){
    personal_mgr_form_ = new PersonalMgrForm(page_setting_);
    page_setting_->addTab(personal_mgr_form_, QString("编辑个人资料"));
    preferences_form_ = new PreferencesForm(page_setting_);
    page_setting_->addTab(preferences_form_, QString("系统偏好设置"));
    session_form_ = new QWidget(page_setting_);
    page_setting_->addTab(session_form_, QString("会话设置"));
}
void MainWorkbench::close_this() {
    /* const QMessageBox::StandardButton reply = QMessageBox::question(this, QString("退出程序"), QString("确定退出吗？"), QMessageBox::Yes | QMessageBox::No); // 弹出确认退出的消息框
    if (QMessageBox::Yes == reply) {
        QApplication::quit();
    } */
    QMessageBox question_box;
    question_box.setWindowTitle("退出系统");
    question_box.setText("是否重新登录？");

    const QPushButton* btn_re_login = question_box.addButton("重新登录", QMessageBox::AcceptRole); // 接受操作, 回车键默认触发
    const QPushButton* btn_quit = question_box.addButton("直接退出", QMessageBox::DestructiveRole); // 危险操作
    const QPushButton* btn_cancel = question_box.addButton("取消", QMessageBox::RejectRole); // 拒绝操作
    question_box.exec(); // 自定义按钮需通过 clickedButton() 获取用户点击的按钮对象

    if (btn_re_login == question_box.clickedButton()) {
        emit sig_btn_clicked(1);
    } else if (btn_quit == question_box.clickedButton()) {
        emit sig_btn_clicked(2);
    } else {
        emit sig_btn_clicked(0);
    }
}
