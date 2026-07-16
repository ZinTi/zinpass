#include "common/numbered_text_edit.h"
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QFontMetrics>

NumberedTextEdit::NumberedTextEdit(QWidget* parent)
    : QPlainTextEdit(parent),
      line_number_area(new class LineNumberArea(this)),
      line_number_font(font()) {
    connect(this, &QPlainTextEdit::blockCountChanged,
            this, &NumberedTextEdit::update_line_number_area_width);
    connect(this, &QPlainTextEdit::updateRequest,
            this, &NumberedTextEdit::update_line_number_area);

    setViewportMargins(line_number_area_width(), 0, 0, 0);
    line_number_font.setPointSize(font().pointSize());
}

void NumberedTextEdit::resizeEvent(QResizeEvent* event) {
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    line_number_area->setGeometry(
        QRect(cr.left(), cr.top(), line_number_area_width(), cr.height())
    );
}

int NumberedTextEdit::line_number_area_width() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    QFontMetrics fm(line_number_font);
    int space = 6 + fm.horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void NumberedTextEdit::update_line_number_area_width(int) {
    setViewportMargins(line_number_area_width(), 0, 0, 0);
}

void NumberedTextEdit::update_line_number_area(const QRect& rect, int dy) {
    if (dy)
        line_number_area->scroll(0, dy);
    else
        line_number_area->update(0, rect.y(), line_number_area->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        update_line_number_area_width(0);
}

void NumberedTextEdit::line_number_area_paint_event(QPaintEvent* event) {
    QPainter painter(line_number_area);
    painter.fillRect(event->rect(), QColor(136, 136, 136));
    painter.setFont(line_number_font);

    QTextBlock block = firstVisibleBlock();
    int block_number = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(block_number + 1);
            painter.setPen(Qt::lightGray);
            painter.drawText(0, top,
                            line_number_area->width() - 3,
                            fontMetrics().height(),
                            Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++block_number;
    }
}
