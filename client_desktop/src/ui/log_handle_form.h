#ifndef LOG_HANDLE_FORM_H
#define LOG_HANDLE_FORM_H

#include <QHBoxLayout>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QTableView>

class LogHandleForm final : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief 默认构造函数
     */
    explicit LogHandleForm(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~LogHandleForm() override;

private:
    void setup_ui();   // 界面初始化函数

    // widgets
    QPushButton* btn_load_;
    QPushButton* btn_clear_;
    QTableView* tb_log_;

    // layout
    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_ctl_;

};

#endif
