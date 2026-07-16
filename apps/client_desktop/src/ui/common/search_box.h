#ifndef SEARCH_BOX_H
#define SEARCH_BOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class SearchBox final : public QWidget {
    Q_OBJECT
public:
    explicit SearchBox(QWidget *parent_ = nullptr);
    ~SearchBox() override;

signals:
    void search_triggered(const QString &keyword_);

private slots:
    void on_search_button_clicked_();

private:
    QHBoxLayout *lyt_;
    QLineEdit *line_edit_;
    QPushButton *search_button_;
};

#endif // SEARCH_BOX_H
