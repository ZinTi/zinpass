#include "tools/pwd_gen_dlg.h"
#include <QApplication>
#include <QDialog>
#include <QMenu>
#include <QPushButton>
#include <QWidgetAction>
#include <QFileDialog>
#include <QClipboard>
#include <QProcess>
#include "business/pwd_generator.h"
#include "common/toast.h"
#include "common/font_util.h"

ComponentsMenuWgt::ComponentsMenuWgt(QWidget *parent) : QWidget(parent){
    btn_components_menu_ = new QPushButton("使用字符", this);

    // 创建菜单
    menu_components_ = new QMenu(this);

    // 创建一个包含四个QCheckBox的QWidget、创建四个QCheckBox并添加到布局中
    this->wgt_chk_box_ = new QWidget(this);
    lyt_chk_box_ = new QVBoxLayout(this->wgt_chk_box_);

    chk_digits_ = new QCheckBox("数字（0-9）", this->wgt_chk_box_);
    chk_lowercase_ = new QCheckBox("小写字母（a-z）", this->wgt_chk_box_);
    chk_uppercase_ = new QCheckBox("大写字母（A-Z）", this->wgt_chk_box_);
    chk_symbols_ = new QCheckBox("特殊字符（~!@#$等）", this->wgt_chk_box_);
    chk_digits_->setChecked(true);
    chk_lowercase_->setChecked(true);
    chk_uppercase_->setChecked(true);
    chk_symbols_->setChecked(true);

    lyt_chk_box_->addWidget(chk_digits_);
    lyt_chk_box_->addWidget(chk_lowercase_);
    lyt_chk_box_->addWidget(chk_uppercase_);
    lyt_chk_box_->addWidget(chk_symbols_);

    // 创建QWidgetAction并将包含QCheckBox的QWidget添加到其中
    this->wgt_action_ = new QWidgetAction(menu_components_);
    this->wgt_action_->setDefaultWidget(this->wgt_chk_box_);

    // 将QWidgetAction添加到菜单中
    menu_components_->addAction(this->wgt_action_);

    // 为按钮绑定菜单
    btn_components_menu_->setMenu(menu_components_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(btn_components_menu_);
    setLayout(lyt_main_);
}

ComponentsMenuWgt::~ComponentsMenuWgt(){
    // 析构函数中不需要手动删除子部件，Qt会自动处理
}

bool ComponentsMenuWgt::isDigitsChecked() const{
    return chk_digits_->isChecked();
}

bool ComponentsMenuWgt::isLowercaseChecked() const{
    return chk_lowercase_->isChecked();
}

bool ComponentsMenuWgt::isUppercaseChecked() const{
    return chk_uppercase_->isChecked();
}

bool ComponentsMenuWgt::isSymbolsChecked() const{
    return chk_symbols_->isChecked();
}

/******************************************************************************/

PwdGenDlg::PwdGenDlg(QWidget *parent) : QDialog(parent){
    initWidget(); // 初始化控件
    initLayout(); // 初始化布局

    setWindowTitle(QString("密码生成器"));  // 设置对话框属性

}

PwdGenDlg::~PwdGenDlg() = default;

void PwdGenDlg::initWidget(){
    l_title_ = new QLabel(QString("密码生成器"), this);
    l_title_->setAlignment(Qt::AlignHCenter);

    txt_display_msg_ = new QTextEdit(QString("说明：<br/>1、长度范围6-99；<br/>2、数量范围1-99；"), this);
    txt_display_msg_->setReadOnly(true);
    txt_display_msg_->setFixedSize(150, 100);

    // 按钮
    btn_begin_ = new QPushButton(QString("生成密码"), this);
    btn_copy_ = new QPushButton(QString("复制"), this);
    btn_clear_ = new QPushButton(QString("清空"), this);
    btn_save_as_ = new QPushButton(QString("另存为"), this);
    btn_cli_mode_ = new QPushButton(QString("命令行模式"), this);

    menu_components_Widget = new ComponentsMenuWgt(this);

    l_length_ = new QLabel(QString("密码长度"), this);
    l_num_ = new QLabel(QString("生成数量"), this);
    e_length_ = new QSpinBox(this);
    e_num_ = new QSpinBox(this);
    rdo_en_colorful_ = new QRadioButton(QString("启用不同颜色区分字符"), this);
    e_length_->setRange(6, 999);
    e_length_->setValue(12);
    e_num_->setRange(1, 99);
    e_num_->setValue(5);
    rdo_en_colorful_->setChecked(true);

    // 显示
    editor_pwd_ = new NumberedTextEdit(this);
    editor_pwd_->setPlaceholderText(QString("生成结果"));
    editor_pwd_->setReadOnly(true);
    editor_pwd_->setFont(FontUtil::instance().codeEditorFont()); // 使用工具类获取字体

    connect(btn_begin_, &QPushButton::clicked, this, &PwdGenDlg::on_btn_begin_clicked);
    connect(btn_copy_, &QPushButton::clicked, this, &PwdGenDlg::on_btn_copy_clicked);
    connect(btn_clear_, &QPushButton::clicked, this, &PwdGenDlg::on_btn_clear_clicked);
    connect(btn_save_as_, &QPushButton::clicked, this, &PwdGenDlg::on_btn_save_as_clicked);
    connect(btn_cli_mode_, &QPushButton::clicked, this, &PwdGenDlg::on_btn_cli_mode_clicked);
}

void PwdGenDlg::initLayout(){
    lyt_main_ = new QVBoxLayout(this);
    lyt_header_ = new QHBoxLayout(this);
    lyt_body_ = new QHBoxLayout(this);
    lyt_footer_ = new QHBoxLayout(this);
    lyt_setting_ = new QVBoxLayout(this);
    lyt_edit_ = new QFormLayout(this);

    lyt_main_->addLayout(lyt_header_);
    lyt_main_->addLayout(lyt_body_);
    lyt_main_->addLayout(lyt_footer_);

    lyt_header_->addWidget(l_title_);

    lyt_setting_->addWidget(txt_display_msg_);
    lyt_setting_->addWidget(menu_components_Widget);
    lyt_setting_->addLayout(lyt_edit_);

    lyt_edit_->addRow(l_length_, e_length_);
    lyt_edit_->addRow(l_num_, e_num_);
    lyt_edit_->addRow(rdo_en_colorful_); // 独占一行

    lyt_body_->addLayout(lyt_setting_);
    lyt_body_->addWidget(editor_pwd_);

    lyt_footer_->addWidget(btn_begin_);
    lyt_footer_->addWidget(btn_copy_);
    lyt_footer_->addWidget(btn_clear_);
    lyt_footer_->addWidget(btn_save_as_);
    lyt_footer_->addWidget(btn_cli_mode_);

    setLayout(lyt_main_);
}

void PwdGenDlg::on_btn_begin_clicked(){
    passwords_.clear();  // 清空之前的数据
    const bool hasDigits = menu_components_Widget->isDigitsChecked();
    const bool hasLowercase = menu_components_Widget->isLowercaseChecked();
    const bool hasUppercase = menu_components_Widget->isUppercaseChecked();
    const bool hasSymbols = menu_components_Widget->isSymbolsChecked();

    if(false == (hasDigits || hasLowercase || hasUppercase || hasSymbols)){
        txt_display_msg_->setTextColor(QColor::fromRgb(255, 0, 0));
        txt_display_msg_->setText(QString("至少选择一种字符"));
        return;
    }

    const int inLength = e_length_->value();
    const int inNum = e_num_->value();
    const bool inColorful = rdo_en_colorful_->isChecked();

    try {
        PwdGenerator pwd_gen(inLength, inNum);
        pwd_gen.setComponentsByBool(hasDigits, hasUppercase, hasLowercase, hasSymbols);

        if(pwd_gen.getPasswords(passwords_)){
            txt_display_msg_->setTextColor(QColor::fromRgb(0, 255, 0));
            txt_display_msg_->setText(QString("生成成功"));
            editor_pwd_->clear();
            if(inColorful){
                // 使用文本光标操作显示彩色字符：数字显示粉红色、小写英文字母黄色、大写英文字母蓝色、特殊字符亮绿色
                QTextCursor cursor(editor_pwd_->document());
                bool firstPassword = true;
                for (const std::string& passwd : passwords_) {
                    if (!firstPassword) {
                        cursor.insertText("\n"); // 在密码之间添加换行
                    }
                    for (const char c : passwd) {
                        QTextCharFormat format;
                        if (std::isdigit(c)) {
                            format.setForeground(QColor::fromRgb(255, 105, 180)); // 粉红色
                        } else if (std::islower(c)) {
                            format.setForeground(QColor::fromRgb(255, 215, 0)); // 金黄色
                        } else if (std::isupper(c)) {
                            format.setForeground(QColor::fromRgb(0, 128, 255)); // 亮蓝色
                        } else {
                            format.setForeground(QColor::fromRgb(50, 205, 50)); // 亮绿色
                        }
                        cursor.insertText(QString(c), format);
                    }
                    firstPassword = false;
                }
            } else {
                // 使用文本光标显示单色字符
                QTextCursor cursor(editor_pwd_->document());
                QTextCharFormat format;
                format.setForeground(QColor::fromRgb(255, 100, 100));
                cursor.setCharFormat(format);
                bool firstPassword = true;
                for(const std::string& passwd : passwords_){
                    if (!firstPassword) {
                        cursor.insertText("\n"); // 在密码之间添加换行
                    }
                    cursor.insertText(QString::fromStdString(passwd));
                    firstPassword = false;
                }
            }
        }else{
            txt_display_msg_->setTextColor(QColor::fromRgb(255, 0, 0));
            txt_display_msg_->setText(QString("生成失败"));
        }
    } catch(const std::exception& e) {
        txt_display_msg_->setTextColor(QColor::fromRgb(255, 0, 0));
        txt_display_msg_->setText(QString("发生异常：%1").arg(e.what()));
    }
}
void PwdGenDlg::on_btn_copy_clicked() const {
    QString clipboard_text;
    for (const std::string& passwd : passwords_) {
        clipboard_text += QString::fromStdString(passwd) + "\n";
    }
    // 移除最后一个换行符
    if (!clipboard_text.isEmpty()) {
        clipboard_text.chop(1);
    }

    // 获取剪贴板并设置文本
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clipboard_text);

    // 显示复制结果
    txt_display_msg_->setTextColor(QColor::fromRgb(0, 255, 0));
    txt_display_msg_->setText(QString("已经全部复制到剪贴板"));
}

void PwdGenDlg::on_btn_clear_clicked() const {
    editor_pwd_->clear();
}

void PwdGenDlg::on_btn_save_as_clicked(){  // Save text to file.
    QString text;
    for (const std::string& passwd : passwords_) {
        text += QString::fromStdString(passwd) + "\n";
    }
    // 移除最后一个换行符
    if (!text.isEmpty()) {
        text.chop(1);
    }

    // 打开文件保存对话框
    const QString file_path = QFileDialog::getSaveFileName(this, "另存为", "",
        "Normal text file (*.txt);;Comma-Separated Values (*.csv);;All types (*)");

    if (!file_path.isEmpty()) {
        // 打开文件进行写入操作
        QFile file(file_path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 使用 QTextStream 写入文本
            QTextStream out(&file);
            out << text;
            // 关闭文件
            file.close();
        }
    }
}

void PwdGenDlg::on_btn_cli_mode_clicked() {
    const QString app_dir = QCoreApplication::applicationDirPath();

#ifdef Q_OS_WIN
    // Windows 解决方案：使用 cmd.exe 显式打开控制台
    const QString program = "cmd.exe";
    QStringList arguments;
    arguments << "/C" << "start" << "cmd.exe" << "/K" << "pwdgen.exe";

    if (!QProcess::startDetached(program, arguments, app_dir)) {
        Toast::showToast(nullptr, "⚠️ Failed to start pwdgen executable", 3000,
                        QColor(200, 0, 0, 220), Qt::white);
    }
#else
    // Linux/macOS 解决方案：使用终端模拟器
    QString terminal;

    // 尝试检测常见的终端模拟器
    QStringList terminals = {
        "x-terminal-emulator",  // Debian/Ubuntu 通用
        "gnome-terminal",      // GNOME
        "konsole",              // KDE
        "xfce4-terminal",       // XFCE
        "lxterminal",           // LXDE
        "mate-terminal",        // MATE
        "tilix",                // Tilix
        "terminator",           // Terminator
        "alacritty",            // Alacritty
        "kitty",                // Kitty
        "wezterm",              // WezTerm
        "xterm"                 // 最基础的终端（通常可用）
    };

    // 查找可用的终端
    for (const QString& term : terminals) {
        if (QFile::exists("/usr/bin/" + term) ||
            !QStandardPaths::findExecutable(term).isEmpty()) {
            terminal = term;
            break;
        }
    }

    if (terminal.isEmpty()) {
        // 找不到终端，尝试直接运行（可能没有窗口）
        if (!QProcess::startDetached("./pwdgen", QStringList(), app_dir)) {
            Toast::showToast(nullptr, "⚠️ Failed to start pwdgen", 3000,
                            QColor(200, 0, 0, 220), Qt::white);
        }
        return;
    }

    // 准备启动命令
    QStringList arguments;
    if (terminal == "gnome-terminal") {
        arguments << "--" << "./pwdgen";
    } else if (terminal == "konsole") {
        arguments << "-e" << "./pwdgen";
    } else if (terminal == "xterm") {
        arguments << "-e" << "./pwdgen";
    } else {
        // 大多数终端使用 -e 参数
        arguments << "-e" << "./pwdgen";
    }

    if (!QProcess::startDetached(terminal, arguments, app_dir)) {
        Toast::showToast(nullptr, "⚠️ Failed to start terminal for pwdgen", 3000,
                        QColor(200, 0, 0, 220), Qt::white);
    }
#endif
}
