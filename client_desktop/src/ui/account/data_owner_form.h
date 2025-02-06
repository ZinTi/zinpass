#ifndef DATA_OWNER_FORM_H
#define DATA_OWNER_FORM_H

#include <QWidget>

class DataOwnerForm final : public QWidget
{
    Q_OBJECT

public:
    explicit DataOwnerForm(QWidget *parent = nullptr);
    ~DataOwnerForm() override;

private:

};

#endif
