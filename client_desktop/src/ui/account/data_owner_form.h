#ifndef DATA_OWNER_FORM_H
#define DATA_OWNER_FORM_H

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

class DataOwnerForm final : public QWidget {
    Q_OBJECT

public:
    explicit DataOwnerForm(QWidget *parent = nullptr);
    ~DataOwnerForm() override;

private:
    void setup_ui();

    QTableView* tv_data_ = nullptr;

    QLabel* lbl_tips_ = nullptr;
    QLabel* lbl_your_key_ = nullptr;
    QLineEdit* edit_your_key_ = nullptr;
    QLabel* lbl_another_username_ = nullptr;
    QLineEdit* edit_another_username_ = nullptr;
    QLabel* lbl_another_key_ = nullptr;
    QLineEdit* edit_another_key_ = nullptr;

    QHBoxLayout* lyt_center_ = nullptr;
    QFormLayout* lyt_form_ = nullptr;
    QVBoxLayout* lyt_main_ = nullptr;
};

#endif
