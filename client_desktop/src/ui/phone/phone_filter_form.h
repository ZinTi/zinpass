#ifndef PHONE_FILTER_FORM_H
#define PHONE_FILTER_FORM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

class PhoneFilterForm final : public QWidget {
    Q_OBJECT

public:
    explicit PhoneFilterForm(QWidget *parent = nullptr);
    ~PhoneFilterForm() override;

    QMap<QString, QVariant> get_form_data() const;

signals:
    void form_submitted();

private slots:
    void on_btn_refresh_clicked() const;          // 处理刷新按键点击事件

private:
    void setup_ui();
    void initial_input_widgets() const;  // 初始化输入控件

    // 基本控件
    QComboBox* combo_telecom_;
    QComboBox* combo_phone_;
    QPushButton* btn_refresh_;
    QPushButton* btn_submit_;
    QGroupBox* box_filter_;

    // std::vector<std::string> telecom_operators_;    // 电信运营商可选项
    // std::vector<std::string> phone_numbers_;        // 手机号可选项

    QHBoxLayout* lyt_main_;
    QHBoxLayout* lyt_gbox_;
    QFormLayout* lyt_form_condition_;
    QVBoxLayout* lyt_btn_;

};

#endif // PHONE_FILTER_FORM_H
