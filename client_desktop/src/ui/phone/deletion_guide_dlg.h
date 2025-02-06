#ifndef DELETION_GUIDE_DLG_H
#define DELETION_GUIDE_DLG_H

#include <QWidget>
#include <QRadioButton>
#include <QDialog>
#include <qdialogbuttonbox.h>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFormLayout>

class DeletionGuideDlg final : public QDialog {
    Q_OBJECT
public:
    explicit DeletionGuideDlg(int phone_id, int affected_records, QWidget *parent = nullptr);
    ~DeletionGuideDlg() override;

    int get_selected_option() const; // 返回 0~3 或 -1（未选）

private slots:
    void on_btn_ok_clicked();

private:
    int phone_id_;                  // phone id to be deleted
    int affected_records_;          // number of records affected by deletion
    int selected_option_;           // selected option from radio buttons

    void setup_ui();
    void initial_combo_box() const;  // 初始化输入控件

    QLabel* l_title_;
    QTextEdit* txt_guide_;

    QComboBox* combo_phone_;

    QButtonGroup* group_rdo_btn_;
    QRadioButton* rdo_option1_;
    QRadioButton* rdo_option2_;
    QRadioButton* rdo_option3_;
    QRadioButton* rdo_option4_;
    QGroupBox* group_box_;

    QDialogButtonBox* dlg_btn_box_;

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_title_;
    QVBoxLayout* lyt_rdo_;
    QHBoxLayout* lyt_rdo_combo_;
};

#endif // DELETION_GUIDE_DLG_H
