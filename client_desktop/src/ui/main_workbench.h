#ifndef MAIN_WORKBENCH_H
#define MAIN_WORKBENCH_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QStackedWidget>
#include <QHBoxLayout>
#include "function_menu_form.h" // 功能菜单
#include "account_mgr_form.h" // 管理账号
#include "phone_mgr_form.h" // 管理手机号
#include "check_pwd_security_form.h" // 检查密码安全
#include "data_owner_form.h" // 管理数据归属
#include "output_form.h"
#include "input_form.h"
#include "log_run_form.h"
#include "log_handle_form.h"
#include "personal_mgr_form.h" // 管理个人账号
#include "preferences_form.h" // 注销账号
#include "pwd_gen_dlg.h"

namespace Ui {
class MainWorkbench;
}

class MainWorkbench final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWorkbench(QWidget *parent = nullptr);
    ~MainWorkbench() override;

signals:
    void sig_btn_clicked(int index); // qt信号，请求 1-重新登录、2-直接退出

private slots:
    // === 文件菜单 ===
    void onNew();
    void onOpen();
    void onSave();

    // === 编辑菜单 ===
    void onCopy();
    void onPaste();
    void onCut();

    // === 查看菜单 ===
    void onZoomIn(); // 放大
    void onZoomOut();// 缩小
    void onResetZoom(); // 重置缩放

    // === 设置菜单 ===
    void onOptions(); // 选项设置
    void onPreferences(); // 偏好设置
    void onCustomize(); // 自定义设置

    // === 工具菜单 ===
    void onPasswdGen(); // 密码生成器
    void onEncrypt(); // 加密
    void onDecrypt(); // 解密

    // === 帮助菜单 ===
    void onHelp(); // 帮助
    void onAbout(); // 关于
    void onCheckUpdates(); // 检查更新

    // 自定义槽函数，用于处理 func_menu_form_ 的按钮点击事件
    void handle_menu_select(int index);

private:
    // 菜单栏和顶级菜单 | 文件 编辑 查看 设置 工具 帮助
    QMenuBar* menu_bar_;
    QMenu* menu_file_;
    QMenu* menu_edit_;
    QMenu* menu_view_;
    QMenu* menu_setting_;
    QMenu* menu_tools_;
    QMenu* menu_help_;

    void setup_ui();
    // ===== 创建菜单 =====
    void create_menus();
    void setup_menu_file();
    void setup_menu_edit();
    void setup_menu_view();
    void setup_menu_setting();
    void setup_menu_tools();
    void setup_menu_help();

    // ===== 菜单动作 =====
    // ------ 文件 ------
    QAction* action_new_file_;
    QAction* action_open_file_;
    QAction* action_save_file_;
    QAction* action_exit_;
    // ------ 编辑 ------
    QAction* action_copy_;
    QAction* action_paste_;
    QAction* action_cut_;
    // ------ 查看 ------
    QAction* action_zoom_in_;
    QAction* action_zoom_out_;
    QAction* action_reset_zoom_;
    // ------ 设置 ------
    QAction* action_options_;
    QAction* action_preferences_;
    QAction* action_customize_;
    // ------ 工具 ------
    QAction* action_passwd_gen_;
    QAction* action_encrypt_;
    QAction* action_decrypt_;
    // ------ 帮助 ------
    QAction* action_help_;
    QAction* action_about_;
    QAction* action_update_;
    // ------------------

    QWidget* central_widget_; // 中心
    QStatusBar* status_bar_; // 状态栏

    QHBoxLayout* lyt_main_;

    FunctionMenuForm* func_menu_form_; // 功能菜单（左侧）
    QStackedWidget* right_stacked_widget_; // 堆叠部件（右侧）

    QTabWidget* page_data_mgr_; // 数据管理-选项卡部件
    QTabWidget* page_transfer_; // 迁移备份-选项卡部件
    QTabWidget* page_log_; // 日志信息-选项卡部件
    QTabWidget* page_setting_; // 个人管理-选项卡部件

    void page_data_mgr_init();
    void page_transfer_init();
    void page_log_init();
    void page_setting_init();
    void close_this();

    AccountMgrForm* acc_mgr_form_; // 管理账号
    PhoneMgrForm* phone_mgr_form_; // 管理手机号
    CheckPwdSecurityForm* chk_pwd_security_form_; // 管理邮箱
    DataOwnerForm* data_owner_form_; // 管理数据归属

    OutputForm* output_form_; // 数据导出
    InputForm* input_form_; // 数据导入

    LogRunForm* log_run_form_; // 运行日志
    LogHandleForm* log_handle_form_; // 操作日志

    PersonalMgrForm* personal_mgr_form_; // 编辑个人资料
    PreferencesForm* preferences_form_; // 偏好设置
    QWidget* session_form_; // 会话设置

};

#endif
