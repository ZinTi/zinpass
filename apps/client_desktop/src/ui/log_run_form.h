#ifndef LOG_RUN_FORM_H
#define LOG_RUN_FORM_H

#include <QLineEdit>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

class LogRunForm final : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief 默认构造函数
     */
    explicit LogRunForm(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~LogRunForm() override;

private:
    void setup_ui();

    // widgets
    QLineEdit* e_main_key_;
    QPushButton* btn_load_;
    QPushButton* btn_save_;
    QTextEdit* e_app_log_;

    // layout
    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_ctl_;
};

#endif
