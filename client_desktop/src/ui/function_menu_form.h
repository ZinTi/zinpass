#ifndef FUNCTION_MENU_FORM_H
#define FUNCTION_MENU_FORM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QToolBox>
#include <QPixmap>
#include <QVBoxLayout>

class FunctionMenuForm final : public QWidget{
    Q_OBJECT

public:
    explicit FunctionMenuForm(QWidget* parent = nullptr);
    ~FunctionMenuForm() override;

signals:
    void sig_btn_clicked(int index); // 携带按钮索引，通知主窗口处理按钮点击事件

private slots:
    void on_btn_pwd_clicked();
    void on_btn_tele_clicked();
    void on_btn_check_clicked();
    void on_btn_owner_clicked();

    void on_btn_out_clicked();
    void on_btn_in_clicked();

    void on_btn_log_run_clicked();
    void on_btn_log_handle_clicked();

    void on_btn_edit_personal_clicked();
    void on_btn_preferences_clicked();
    void on_btn_session_clicked();
    void on_btn_exit_clicked();

private:
    QLabel* l_title_;
    QFrame* line_;
    QToolBox* toolbox_func_menu_;
    QWidget* page_data_mgr_;          // 数据管理
    QWidget* page_migrate_;           // 迁移备份
    QWidget* page_log_;               // 日志信息
    QWidget* page_personal_;          // 个人信息

    QPushButton* btn_pwd_;
    QPushButton* btn_tele_;
    QPushButton* btn_check_;
    QPushButton* btn_owner_;

    QPushButton* btn_out_;
    QPushButton* btn_in_;

    QPushButton* btn_log_run_;
    QPushButton* btn_log_handle_;

    QPushButton* btn_edit_personal_;
    QPushButton* btn_preferences_;
    QPushButton* btn_session_;
    QPushButton* btn_exit_;

    QLabel* l_mascot_;
    QPixmap* pix_mascot_;

    // layout
    QVBoxLayout* lyt_main_;
    QVBoxLayout* lyt_data_mgr_;
    QVBoxLayout* lyt_migrate_;
    QVBoxLayout* lyt_log_;
    QVBoxLayout* lyt_personal_;

    void setup_ui();

};

#endif // FUNCTION_MENU_FORM_H
