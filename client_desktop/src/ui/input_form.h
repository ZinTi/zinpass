#ifndef INPUT_FORM_H
#define INPUT_FORM_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class InputForm final : public QWidget
{
    Q_OBJECT

public:
    explicit InputForm(QWidget *parent = nullptr);
    ~InputForm() override;

private:
    void setup_ui();

    QTextEdit* edit_info_;

    QVBoxLayout* lyt_main_;
};

#endif // INPUT_FORM_H
