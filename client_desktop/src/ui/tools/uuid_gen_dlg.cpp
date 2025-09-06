#include "tools/uuid_gen_dlg.h"

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QPushButton>
#include <vector>
#include "common/toast.h"
#include "business/uuid_generator.h"

UuidGenDlg::UuidGenDlg(QWidget *parent) : QDialog(parent) {
    this->setWindowTitle("UUID");

    this->uppercase_ = false;
    this->with_hyphens_ = true;

    // widget
    lbl_uuid_version_ = new QLabel("版本", this);
    this->cb_uuid_version_ = new QComboBox(this);
    this->cb_uuid_version_->addItem("v7");
    this->cb_uuid_version_->addItem("v5");
    this->cb_uuid_version_->addItem("v4");
    this->cb_uuid_version_->setCurrentText("v4");
    this->cb_uuid_version_->setDisabled(true);

    btn_uppercase_ = new QToolButton(this);
    btn_uppercase_->setText("a↓");
    btn_uppercase_->setToolTip(tr("字母大小写"));
    btn_uppercase_->setAutoRaise(true);         // 启用自动悬停效果

    btn_with_hyphens_ = new QToolButton(this);
    btn_with_hyphens_->setIcon(QIcon(":/tools/svg/hyphen.svg"));
    btn_with_hyphens_->setToolTip(tr("连字符"));
    btn_with_hyphens_->setIconSize(QSize(16, 16));
    btn_with_hyphens_->setAutoRaise(true);

    btn_download_ = new QToolButton(this);
    btn_download_->setIcon(QIcon(":/tools/svg/download.svg"));
    btn_download_->setToolTip(tr("下载"));
    btn_download_->setIconSize(QSize(16, 16));
    btn_download_->setAutoRaise(true);

    btn_copy_ = new QToolButton(this);
    btn_copy_->setIcon(QIcon(":/tools/svg/copy.svg"));
    btn_copy_->setToolTip(tr("复制"));
    btn_copy_->setIconSize(QSize(16, 16));
    btn_copy_->setAutoRaise(true);

    lbl_quantity_ = new QLabel("数量", this);
    this->spn_quantity_ = new QSpinBox(this);
    this->spn_quantity_->setRange(1, 100);
    this->spn_quantity_->setValue(1);

    btn_generate_ = new QPushButton("生成", this);

    editor_ = new NumberedTextEdit(this);
    editor_->setReadOnly(true);
    // 这两个好像还是没啥用
    editor_->setLineWrapMode(QPlainTextEdit::WidgetWidth); // 基于控件宽度换行
    editor_->setWordWrapMode(QTextOption::WrapAnywhere);   // 在任何字符后换行

    // layout
    lyt_main_ = new QVBoxLayout(this);
    lyt_top_ = new QHBoxLayout(this);
    lyt_bottom_ = new QHBoxLayout(this);

    lyt_top_->addWidget(lbl_uuid_version_);
    lyt_top_->addWidget(this->cb_uuid_version_);
    lyt_top_->addStretch();
    lyt_top_->addWidget(btn_uppercase_);
    lyt_top_->addWidget(btn_with_hyphens_);
    lyt_top_->addWidget(btn_download_);
    lyt_top_->addWidget(btn_copy_);

    lyt_bottom_->addWidget(lbl_quantity_);
    lyt_bottom_->addWidget(this->spn_quantity_);
    lyt_bottom_->addStretch();
    lyt_bottom_->addWidget(btn_generate_);

    lyt_main_->addLayout(lyt_top_);
    lyt_main_->addWidget(editor_);
    lyt_main_->addLayout(lyt_bottom_);

    connect(btn_uppercase_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_uppercase_clicked);
    connect(btn_with_hyphens_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_with_hyphens_clicked);
    connect(btn_download_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_download_clicked);
    connect(btn_copy_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_copy_clicked);
    connect(btn_generate_, &QPushButton::clicked, this, &UuidGenDlg::on_btn_generate_clicked);

}

UuidGenDlg::~UuidGenDlg() = default;

void UuidGenDlg::on_btn_uppercase_clicked() {
    uppercase_ = !uppercase_;
    if (uppercase_) {
        btn_uppercase_->setText("A↑");
    }else {
        btn_uppercase_->setText("a↓");
    }
}

void UuidGenDlg::on_btn_with_hyphens_clicked() {
    with_hyphens_ = !with_hyphens_;
    if (with_hyphens_) {
        btn_with_hyphens_->setIcon(QIcon(":/tools/svg/hyphen.svg"));
    }else {
        btn_with_hyphens_->setIcon(QIcon(":/tools/svg/no_hyphen.svg"));
    }
}
void UuidGenDlg::on_btn_download_clicked() {
    const QString text = editor_->toPlainText();
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
void UuidGenDlg::on_btn_copy_clicked() {
    const QString clipboard_text = editor_->toPlainText();
    // 获取剪贴板并设置文本
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clipboard_text);
    Toast::showToast(this, QString("已复制到剪贴板"), 1200);
}
void UuidGenDlg::on_btn_generate_clicked() {
    // const QString version = this->cb_uuid_version_->currentText();
    const int count = this->spn_quantity_->value();

    std::vector<std::string> uuids;
    QString uuids_qs;
    for (int i = 0; i < count; i++) {
        /*
        std::array<uint8_t, 16> uuid{};
        std::string name = "zinpass";

        if (version == "v7") {
            uuid = UuidGenerator::v7();
        }else if (version == "v5") {
            uuid = UuidGenerator::v5(UuidGenerator::NAMESPACE_URL, name + std::to_string(i));
        }else if (version == "v4") {
            uuid = UuidGenerator::v4();
        }else {
            Toast::showToast(this, "无效的UUID版本选择", 1200);
            return;
        }

        // 格式化为字符串并输出
        std::string uuid_str = UuidGenerator::toString(uuid, with_hyphens_);
        uuids.push_back(uuid_str);
        uuids_qs.append(uuid_str + "\n");*/
    }
    editor_->setPlainText(uuids_qs);
}