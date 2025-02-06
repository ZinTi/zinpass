#ifndef OUTPUT_FORM_H
#define OUTPUT_FORM_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class OutputForm final : public QWidget{
    Q_OBJECT

public:
    explicit OutputForm(QWidget *parent = nullptr);
    ~OutputForm() override;

private:
    void setup_ui();

    QTextEdit* edit_info_;

    // lyt
    QVBoxLayout* lyt_main_;
};

#endif
