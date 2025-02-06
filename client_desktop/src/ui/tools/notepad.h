#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QPushButton>

#include "common/numbered_text_edit.h"

class Notepad final : public QWidget {
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad() override;

private slots:
    // === 文件菜单 ===
    void new_file();
    void open_file();
    void save_file();

private:
    void setup_ui();
    void setup_menus();

    QMenuBar* menu_bar_ = nullptr;     // 菜单栏
    QStatusBar* status_bar_ = nullptr; // 状态栏

    QPushButton* btn_diff_ = nullptr;
    QPushButton* btn_setting_ = nullptr;

    NumberedTextEdit* editor1_ = nullptr;
    NumberedTextEdit* editor2_ = nullptr;

    QVBoxLayout* lyt_main_ = nullptr;
    QHBoxLayout* lyt_features_ = nullptr;
    QHBoxLayout* lyt_editors_ = nullptr;
};

#endif // NOTEPAD_H
