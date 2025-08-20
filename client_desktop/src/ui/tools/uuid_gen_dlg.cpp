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
    this->lbl_uuid_version_ = new QLabel("版本", this);
    this->cb_uuid_version_ = new QComboBox(this);
    this->cb_uuid_version_->addItem("v7");
    this->cb_uuid_version_->addItem("v5");
    this->cb_uuid_version_->addItem("v4");
    this->cb_uuid_version_->setCurrentText("v4");
    this->cb_uuid_version_->setDisabled(true);

    this->btn_uppercase_ = new QToolButton(this);
    this->btn_uppercase_->setText("a↓");
    this->btn_uppercase_->setToolTip(tr("字母大小写"));
    this->btn_uppercase_->setAutoRaise(true);         // 启用自动悬停效果

    this->btn_with_hyphens_ = new QToolButton(this);
    this->btn_with_hyphens_->setIcon(QIcon(":/tools/svg/hyphen.svg"));
    this->btn_with_hyphens_->setToolTip(tr("连字符"));
    this->btn_with_hyphens_->setIconSize(QSize(16, 16));
    this->btn_with_hyphens_->setAutoRaise(true);

    this->btn_download_ = new QToolButton(this);
    this->btn_download_->setIcon(QIcon(":/tools/svg/download.svg"));
    this->btn_download_->setToolTip(tr("下载"));
    this->btn_download_->setIconSize(QSize(16, 16));
    this->btn_download_->setAutoRaise(true);

    this->btn_copy_ = new QToolButton(this);
    this->btn_copy_->setIcon(QIcon(":/tools/svg/copy.svg"));
    this->btn_copy_->setToolTip(tr("复制"));
    this->btn_copy_->setIconSize(QSize(16, 16));
    this->btn_copy_->setAutoRaise(true);

    this->lbl_quantity_ = new QLabel("数量", this);
    this->spn_quantity_ = new QSpinBox(this);
    this->spn_quantity_->setRange(1, 100);
    this->spn_quantity_->setValue(1);

    this->btn_generate_ = new QPushButton("生成", this);

    this->editor_ = new NumberedTextEdit(this);
    this->editor_->setReadOnly(true);

    // layout
    this->lyt_main_ = new QVBoxLayout(this);
    this->lyt_top_ = new QHBoxLayout(this);
    this->lyt_bottom_ = new QHBoxLayout(this);

    this->lyt_top_->addWidget(this->lbl_uuid_version_);
    this->lyt_top_->addWidget(this->cb_uuid_version_);
    this->lyt_top_->addStretch();
    this->lyt_top_->addWidget(this->btn_uppercase_);
    this->lyt_top_->addWidget(this->btn_with_hyphens_);
    this->lyt_top_->addWidget(this->btn_download_);
    this->lyt_top_->addWidget(this->btn_copy_);

    this->lyt_bottom_->addWidget(this->lbl_quantity_);
    this->lyt_bottom_->addWidget(this->spn_quantity_);
    this->lyt_bottom_->addStretch();
    this->lyt_bottom_->addWidget(this->btn_generate_);

    this->lyt_main_->addLayout(this->lyt_top_);
    this->lyt_main_->addWidget(this->editor_);
    this->lyt_main_->addLayout(this->lyt_bottom_);

    connect(this->btn_uppercase_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_uppercase_clicked);
    connect(this->btn_with_hyphens_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_with_hyphens_clicked);
    connect(this->btn_download_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_download_clicked);
    connect(this->btn_copy_, &QToolButton::clicked, this, &UuidGenDlg::on_btn_copy_clicked);
    connect(this->btn_generate_, &QPushButton::clicked, this, &UuidGenDlg::on_btn_generate_clicked);

}

UuidGenDlg::~UuidGenDlg() = default;

void UuidGenDlg::on_btn_uppercase_clicked() {
    uppercase_ = !uppercase_;
    if (uppercase_) {
        this->btn_uppercase_->setText("A↑");
    }else {
        this->btn_uppercase_->setText("a↓");
    }
}

void UuidGenDlg::on_btn_with_hyphens_clicked() {
    with_hyphens_ = !with_hyphens_;
    if (with_hyphens_) {
        this->btn_with_hyphens_->setIcon(QIcon(":/tools/svg/hyphen.svg"));
    }else {
        this->btn_with_hyphens_->setIcon(QIcon(":/tools/svg/no_hyphen.svg"));
    }
}
void UuidGenDlg::on_btn_download_clicked() {
    const QString text = this->editor_->toPlainText();
    // 打开文件保存对话框
    const QString file_path = QFileDialog::getSaveFileName(this, "另存为", "", "文本文件 (*.txt)");

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
    const QString clipboard_text = this->editor_->toPlainText();
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
    this->editor_->setPlainText(uuids_qs);
}