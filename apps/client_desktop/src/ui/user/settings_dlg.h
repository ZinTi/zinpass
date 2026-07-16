#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QPixmap>
#include <QLineEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QTextEdit>
#include <QVBoxLayout>

// 登录对话框 设置菜单
class SettingsDlg final : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDlg(QWidget* parent = nullptr);
    ~SettingsDlg() override;

private slots:
    void on_btn_ping_clicked();
    void on_btn_reset_clicked();
    void on_btn_refresh_clicked();
    void on_btn_apply_clicked();
    void on_btn_save_clicked();

private:
    std::string ip_address_;
    int port_;

    QTextEdit* e_display_;

    QLabel* l_theme_;
    QLabel* l_language_;
    QLabel* l_ip_address_;
    QLabel* l_port_;
    QLineEdit* e_ip_address_;
    QLineEdit* e_port_;

    QButtonGroup* bg_theme_;
    QRadioButton* btn_dark_mode_; // 深色主题开关
    QRadioButton* btn_light_mode_; // 浅色主题开关
    QRadioButton* btn_system_mode_; // 跟随系统主题开关

    QComboBox* combo_language_; // 语言选择

    QPushButton* btn_ping_;     // ping 服务器测试网络连接
    QPushButton* btn_reset_;    // 重置
    QPushButton* btn_refresh_;  // 刷新
    QPushButton* btn_apply_;        // 应用
    QPushButton* btn_save_;     // 保存

    QVBoxLayout* lyt_main_;
    QGridLayout* lyt_grid_;
    QHBoxLayout* lyt_btn_;

    void setup_ui();
};


#endif
