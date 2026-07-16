#ifndef UUID_GEN_DLG_H
#define UUID_GEN_DLG_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "common/numbered_text_edit.h"

class UuidGenDlg final : public QDialog {
public:
    explicit UuidGenDlg(QWidget *parent = nullptr);
    ~UuidGenDlg() override;

private slots:
    void on_btn_uppercase_clicked();
    void on_btn_with_hyphens_clicked();
    void on_btn_download_clicked();
    void on_btn_copy_clicked();
    void on_btn_generate_clicked();

private:
    bool uppercase_;
    bool with_hyphens_;  // 有连字符

    // widget
    QLabel* lbl_uuid_version_;
    QComboBox* cb_uuid_version_;

    QToolButton* btn_uppercase_;
    QToolButton* btn_with_hyphens_;
    QToolButton* btn_download_;
    QToolButton* btn_copy_;

    QLabel* lbl_quantity_;
    QSpinBox* spn_quantity_;

    QPushButton* btn_generate_;

    NumberedTextEdit* editor_;

    // layout
    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_top_;
    QHBoxLayout* lyt_bottom_;

};

#endif
