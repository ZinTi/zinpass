#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>

class PreferencesForm final : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesForm(QWidget *parent = nullptr);
    ~PreferencesForm() override;

private:
};

#endif
