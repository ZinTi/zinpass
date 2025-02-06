#include "function_menu_form.h"

FunctionMenuForm::FunctionMenuForm(QWidget *parent) : QWidget(parent){
    this->setup_ui();
}

FunctionMenuForm::~FunctionMenuForm(){

}

void FunctionMenuForm::setup_ui() {
    // 主窗口设置
    // this->setFixedSize(162, 426);
    // this->setWindowTitle("Form");

    // 标题标签
    this->l_title_ = new QLabel("功能菜单", this);
    const QFont titleFont("站酷酷黑", 16, QFont::Bold);
    this->l_title_->setFont(titleFont);
    this->l_title_->setAlignment(Qt::AlignCenter);

    // 创建分隔线
    this->line_ = new QFrame(this);
    this->line_->setFrameShape(QFrame::HLine);
    this->line_->setFrameShadow(QFrame::Sunken);

    // 功能菜单工具箱
    this->toolbox_func_menu_ = new QToolBox(this);
    this->toolbox_func_menu_->setMaximumHeight(300);

    // 2.1. 数据管理页面
    this->page_data_mgr_ = new QWidget(this);
    this->btn_pwd_ = new QPushButton("密码维护", this->page_data_mgr_);
    this->btn_tele_ = new QPushButton("手机号码", this->page_data_mgr_);
    this->btn_check_ = new QPushButton("安全检查", this->page_data_mgr_);
    this->btn_owner_ = new QPushButton("数据归属", this->page_data_mgr_);

    // 2.2. 迁移备份页面
    this->page_migrate_ = new QWidget(this);
    this->btn_out_ = new QPushButton("数据导出", this->page_migrate_);
    this->btn_in_ = new QPushButton("数据导入", this->page_migrate_);

    // 2.3. 日志信息页面
    this->page_log_ = new QWidget(this);
    this->btn_log_run_ = new QPushButton("运行日志", this->page_log_);
    this->btn_log_handle_ = new QPushButton("操作日志", this->page_log_);

    // 2.4. 个人管理页面
    this->page_personal_ = new QWidget(this);
    this->btn_edit_personal_ = new QPushButton("编辑资料", this->page_personal_);
    this->btn_preferences_ = new QPushButton("偏好设置", this->page_personal_);
    this->btn_session_ = new QPushButton("会话设置", this->page_personal_);
    this->btn_exit_ = new QPushButton("退出系统", this->page_personal_);
    this->btn_exit_->setStyleSheet("background-color: red;");

    // mascot
    this->l_mascot_ = new QLabel(this); // 显示 mascot
    this->pix_mascot_ = new QPixmap(":/icon/mascot.png");
    if (this->pix_mascot_->isNull()) {
        // qDebug() << "Failed to load logo from resource.";
        l_mascot_->setText("Mascot");
    } else {
        constexpr int max_height = 72;
        const int width = pix_mascot_->width() * max_height / pix_mascot_->height();
        l_mascot_->setFixedSize(width, max_height);
        l_mascot_->setPixmap(pix_mascot_->scaled(l_mascot_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)); // 使用平滑转换模式
    }

    // 布局
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_main_->setContentsMargins(12, 12, 12, 12);
    this->lyt_main_->setSpacing(15);
    this->lyt_main_->addWidget(this->l_title_);
    this->lyt_main_->addWidget(this->line_);
    this->lyt_main_->addWidget(this->toolbox_func_menu_);
    this->lyt_main_->addStretch();  // 添加一个可伸缩的空间（stretch）, 尽可能地扩展以填充可用空间，从而将其他部件推向布局的一端
    this->lyt_main_->addWidget(this->l_mascot_);

    this->lyt_data_mgr_ = new QVBoxLayout(page_data_mgr_);
    this->lyt_data_mgr_->setContentsMargins(8, 8, 8, 8);
    this->lyt_data_mgr_->setSpacing(3);
    this->lyt_data_mgr_->addWidget(this->btn_pwd_);
    this->lyt_data_mgr_->addWidget(this->btn_tele_);
    this->lyt_data_mgr_->addWidget(this->btn_check_);
    this->lyt_data_mgr_->addWidget(this->btn_owner_);

    this->lyt_migrate_ = new QVBoxLayout(page_migrate_);
    this->lyt_migrate_->setContentsMargins(8, 8, 8, 8);
    this->lyt_migrate_->setSpacing(3);
    this->lyt_migrate_->addWidget(this->btn_out_);
    this->lyt_migrate_->addWidget(this->btn_in_);

    this->lyt_log_ = new QVBoxLayout(page_log_);
    this->lyt_log_->setContentsMargins(8, 8, 8, 8);
    this->lyt_log_->setSpacing(3);
    this->lyt_log_->addWidget(this->btn_log_run_);
    this->lyt_log_->addWidget(this->btn_log_handle_);

    this->lyt_personal_ = new QVBoxLayout(page_personal_);
    this->lyt_personal_->setContentsMargins(8, 8, 8, 8);
    this->lyt_personal_->setSpacing(3);
    this->lyt_personal_->addWidget(this->btn_edit_personal_);
    this->lyt_personal_->addWidget(this->btn_preferences_);
    this->lyt_personal_->addWidget(this->btn_session_);
    this->lyt_personal_->addWidget(this->btn_exit_);

    // 添加页面到工具箱
    this->toolbox_func_menu_->addItem(this->page_data_mgr_, QIcon(":/btn/btn/account.png"),"数据管理");
    this->toolbox_func_menu_->addItem(this->page_migrate_,  QIcon(":/btn/btn/bus.png"), "迁移备份");
    this->toolbox_func_menu_->addItem(this->page_log_, QIcon(":/btn/btn/analytics.png"), "日志信息");
    this->toolbox_func_menu_->addItem(this->page_personal_, QIcon(":/btn/btn/gear.png"), "个人管理");
    this->toolbox_func_menu_->setCurrentIndex(3);   // 设置默认打开的页面

    connect(this->btn_pwd_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_pwd_clicked);
    connect(this->btn_tele_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_tele_clicked);
    connect(this->btn_check_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_check_clicked);
    connect(this->btn_owner_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_owner_clicked);

    connect(this->btn_out_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_out_clicked);
    connect(this->btn_in_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_in_clicked);

    connect(this->btn_log_run_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_log_run_clicked);
    connect(this->btn_log_handle_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_log_handle_clicked);

    connect(this->btn_edit_personal_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_edit_personal_clicked);
    connect(this->btn_preferences_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_preferences_clicked);
    connect(this->btn_session_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_session_clicked);
    connect(this->btn_exit_, &QPushButton::clicked, this, &FunctionMenuForm::on_btn_exit_clicked);
}

void FunctionMenuForm::on_btn_pwd_clicked() {
    emit sig_btn_clicked(1); // 发出自定义信号，携带按钮索引 1
}
void FunctionMenuForm::on_btn_tele_clicked(){emit sig_btn_clicked(2);}
void FunctionMenuForm::on_btn_check_clicked(){emit sig_btn_clicked(3);}
void FunctionMenuForm::on_btn_owner_clicked(){emit sig_btn_clicked(4);}

void FunctionMenuForm::on_btn_out_clicked(){emit sig_btn_clicked(5);}
void FunctionMenuForm::on_btn_in_clicked(){emit sig_btn_clicked(6);}

void FunctionMenuForm::on_btn_log_run_clicked(){emit sig_btn_clicked(7);}
void FunctionMenuForm::on_btn_log_handle_clicked(){emit sig_btn_clicked(8);}

void FunctionMenuForm::on_btn_edit_personal_clicked(){emit sig_btn_clicked(9);}
void FunctionMenuForm::on_btn_preferences_clicked(){emit sig_btn_clicked(10);}
void FunctionMenuForm::on_btn_session_clicked(){emit sig_btn_clicked(11);}
void FunctionMenuForm::on_btn_exit_clicked(){emit sig_btn_clicked(12);}
