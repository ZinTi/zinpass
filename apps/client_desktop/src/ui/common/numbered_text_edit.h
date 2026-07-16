/**
 * 带行号的文本编辑控件
 */
#ifndef NUMBERED_TEXT_EDIT_H
#define NUMBERED_TEXT_EDIT_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QObject>
#include <QTextBlock>

// 1. 行号区域类
class LineNumberArea;

// 2. 带行号的文本编辑器类
class NumberedTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit NumberedTextEdit(QWidget* parent = nullptr);
    void line_number_area_paint_event(QPaintEvent* event);
    int line_number_area_width();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void update_line_number_area_width(int new_block_count);
    void update_line_number_area(const QRect& rect, int dy);

private:
    QWidget* line_number_area;
    QFont line_number_font;
};

// 行号区域类的实现
class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(NumberedTextEdit* editor) : QWidget(editor), text_edit(editor) {}
    QSize sizeHint() const override { return QSize(text_edit->line_number_area_width(), 0); }

protected:
    void paintEvent(QPaintEvent* event) override { text_edit->line_number_area_paint_event(event); }

private:
    NumberedTextEdit* text_edit;
};

#endif
