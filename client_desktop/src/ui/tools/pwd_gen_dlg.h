#ifndef PWD_GEN_DLG_H
#define PWD_GEN_DLG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QMenu>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QWidgetAction>
#include "common/numbered_text_edit.h"

// 1/2
class ComponentsMenuWgt final : public QWidget {
public:
    explicit ComponentsMenuWgt(QWidget *parent = nullptr);
    ~ComponentsMenuWgt() override;

    [[nodiscard]] bool isDigitsChecked() const;
    [[nodiscard]] bool isLowercaseChecked() const;
    [[nodiscard]] bool isUppercaseChecked() const;
    [[nodiscard]] bool isSymbolsChecked() const;

private:
    QPushButton* btn_components_menu_;
    QMenu* menu_components_;
    QCheckBox* chk_digits_;
    QCheckBox* chk_lowercase_;
    QCheckBox* chk_uppercase_;
    QCheckBox* chk_symbols_;

    QWidgetAction* wgt_action_;
    QWidget* wgt_chk_box_;
    
    QVBoxLayout* lyt_main_;
    QVBoxLayout* lyt_chk_box_;
};

// 2/2
class PwdGenDlg final : public QDialog{
public:
    explicit PwdGenDlg(QWidget *parent = nullptr);
    ~PwdGenDlg() override;

private slots:
    void on_btn_begin_clicked(); // 生成密码
    void on_btn_copy_clicked() const; // 复制结果
    void on_btn_clear_clicked() const; // 清空
    void on_btn_save_as_clicked(); // 另存为

    static void on_btn_cli_mode_clicked(); // 命令行模式

private:
    std::vector<std::string> passwords_; // 密码列表

    QLabel* l_title_; // 标题
    QTextEdit* txt_display_msg_; // 消息显示

    // 按钮
    QPushButton* btn_begin_; // 生成密码
    QPushButton* btn_copy_; // 复制结果
    QPushButton* btn_clear_; // 清空
    QPushButton* btn_save_as_;
    QPushButton* btn_cli_mode_;

    // 设置下拉菜单
    ComponentsMenuWgt* menu_components_Widget;

    QLabel* l_length_;
    QLabel* l_num_;
    QSpinBox* e_length_;
    QSpinBox* e_num_;
    QRadioButton* rdo_en_colorful_;

    // 密码显示
    NumberedTextEdit* editor_pwd_; // 带行号的文本编辑器用作密码显示

    // 布局
    QVBoxLayout* lyt_main_; // 主框架
    QHBoxLayout* lyt_header_; // 1、header
    QHBoxLayout* lyt_body_; // 2、body
    QHBoxLayout* lyt_footer_; // 3、footer
    QVBoxLayout* lyt_setting_; // 2.1 设置
    QFormLayout* lyt_edit_; // 2.1.2

    void initWidget();
    void initLayout();
};

#endif
