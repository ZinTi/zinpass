#include "login_dlg.h"
#include <QMessageBox>
#include <state_manager.h>
#include "signup_dlg.h"
#include "user.h"
#include "user_rpc.h"
#include "auth_rpc.h"
#include "state_manager.h"

LoginDlg::LoginDlg(QWidget* parent) : QDialog(parent) {
    setup_ui();

    refresh_combo_username(); // 刷新登录Username列表
}

LoginDlg::~LoginDlg() {
}

void LoginDlg::setup_ui() {
    this->resize(360, 480);
    this->setWindowTitle("登录 | ZinPass");

    this->settings_dlg_ = new SettingsDlg(this);

    this->btn_settings_ = new QToolButton(this);
    this->btn_settings_->setIcon(QIcon(":/icon/svg/gear_setting.svg"));
    this->btn_settings_->setToolTip(tr("Settings"));
    this->btn_settings_->setIconSize(QSize(24, 24));  // 标准尺寸
    this->btn_settings_->setAutoRaise(true);         // 启用自动悬停效果

    this->l_title_ = new QLabel("ZinPass 守密司", this);
    const QFont font_title("字酷堂石刻体", 26, QFont::Bold);
    this->l_title_->setFont(font_title);
    this->l_title_->setAlignment(Qt::AlignCenter);

    this->l_logo_ = new QLabel(this);
    this->pixmap_logo_ = new QPixmap(":/icon/spider_128.png");
    if (this->pixmap_logo_->isNull()) {
        this->l_logo_->setText("- Failed to load LOGO! -");
        // qDebug() << "Failed to load logo from resource.";
    } else {
        constexpr int max_height = 128;
        const int width = this->pixmap_logo_->width() * max_height / this->pixmap_logo_->height();
        this->l_logo_->setFixedSize(width, max_height);
        this->l_logo_->setPixmap(this->pixmap_logo_->scaled(this->l_logo_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)); // 使用平滑转换模式
    }

    const QFont font_common("Microsoft YaHei UI", 12);
    this->l_username_ = new QLabel("账号", this);
    this->l_username_->setFont(font_common);
    this->l_key_ = new QLabel("密钥", this);
    this->l_key_->setFont(font_common);

    constexpr int max_width = 220;
    this->combo_username_ = new QComboBox(this);
    this->combo_username_->setFont(font_common);
    this->combo_username_->setMaximumWidth(max_width);
    this->e_key_ = new QLineEdit(this);
    this->e_key_->setFont(font_common);
    this->e_key_->setMaximumWidth(max_width);
    this->e_key_->setEchoMode(QLineEdit::Password);
    this->e_key_->setMaxLength(24); // 密钥最大字符长度

    this->btn_login_ = new QPushButton("登录", this);
    this->btn_login_->setMaximumWidth(100);
    this->btn_signup_ = new QPushButton("注册", this);
    this->btn_signup_->setMaximumWidth(100);

    this->l_copyleft_ = new QLabel("Copyleft 🄯 2025 曾来. GPLv3协议.", this);
    this->l_copyleft_->setFont(font_common);
    this->l_copyleft_->setAlignment(Qt::AlignCenter);

    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_top_ = new QHBoxLayout(this);
    this->lyt_form_ = new QFormLayout(this);
    this->lyt_btn_ = new QHBoxLayout(this);

    // this->lyt_main_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->lyt_top_->addStretch();
    this->lyt_top_->addWidget(this->btn_settings_);
    this->lyt_main_->addLayout(this->lyt_top_);
    this->lyt_main_->addWidget(this->l_title_);
    this->lyt_main_->addWidget(this->l_logo_, 0, Qt::AlignCenter);
    this->lyt_form_->addRow(this->l_username_, this->combo_username_);
    this->lyt_form_->addRow(this->l_key_, this->e_key_);
    this->lyt_form_->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    this->lyt_form_->setLabelAlignment(Qt::AlignRight); // 标签对齐方式
    this->lyt_main_->addLayout(this->lyt_form_);
    this->lyt_btn_->setSpacing(10); // 设置固定间距
    lyt_btn_->addStretch();
    this->lyt_btn_->addWidget(this->btn_login_);
    this->lyt_btn_->addWidget(this->btn_signup_);
    lyt_btn_->addStretch();
    this->lyt_main_->addLayout(this->lyt_btn_);
    this->lyt_main_->addWidget(this->l_copyleft_);

    connect(this->btn_settings_, &QToolButton::clicked, this, &LoginDlg::on_btn_settings_clicked);
    connect(this->btn_login_, &QPushButton::clicked, this, &LoginDlg::on_btn_login_clicked);
    connect(this->btn_signup_, &QPushButton::clicked, this, &LoginDlg::on_btn_signup_clicked);
}

void LoginDlg::refresh_combo_username() const {
    this->combo_username_->clear();
    std::string msg;
    std::vector<std::string> usernames;

    // 创建gRPC通道连接到服务器
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    const zinpass::rpc::UserRPC user_rpc(channel);
    user_rpc.list_usernames(usernames);

    // 添加到组件中
    for (const std::string& str : usernames) {
        this->combo_username_->addItem(QString::fromStdString(str));
    }
}

void LoginDlg::on_btn_settings_clicked() const {
    this->settings_dlg_->exec();
}

void LoginDlg::on_btn_login_clicked() {
    auto user = zinpass::models::SystemUser(); // 用户
    // 获取用户名和输入框的内容
    const QString in_username = this->combo_username_->currentText();
    const QString in_password = this->e_key_->text();
    // const QString in_captcha_code = ui->lineEditCaptcha->text();

    // 执行登录操作
    // 创建gRPC通道连接到服务器
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AuthRPC auth_rpc(channel);
    const std::map<std::string, std::string> res = auth_rpc.login(in_username.toStdString(), in_password.toStdString(), "captcha-code: 0123");

    // 反序列化 response
    zinpass::state::StateManager::UserState state;

    try {
        state.session_id = res.at("session_id");
        state.user_id = res.at("user_id");
        state.username = res.at("username");
        state.nickname = res.at("nickname");
        // state.role = ;
    } catch (const std::out_of_range& e){
        QMessageBox::warning(this, QString("登录失败"), QString("会话建立失败: ") + QString::fromStdString(e.what()));
        return;
    }
    zinpass::state::StateManager::instance().setUserState(state);

    if(zinpass::state::StateManager::instance().getUserState().session_id.size() == 36){
        this->e_key_->clear();
        accept(); // 登录成功，关闭登录对话框
    } else {
        std::string message;
        try{
            message = res.at("message");
        }catch(const std::out_of_range& e){
            message = "无消息";
        }

        QMessageBox::warning(this, QString("登录失败"), QString::fromStdString(message)); // 登录失败，显示错误消息
    }

}

void LoginDlg::on_btn_signup_clicked() {
    SignupDlg signup_dlg(this);
    signup_dlg.exec();
    refresh_combo_username();
}
