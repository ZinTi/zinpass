#ifndef PHONE_DETAIL_FRAME_H
#define PHONE_DETAIL_FRAME_H

#include <QFrame>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include "models/mobile_phone.h"

class PhoneDetailFrame final : public QFrame
{
    Q_OBJECT

public:
    explicit PhoneDetailFrame(QFrame *parent = nullptr);
    ~PhoneDetailFrame() override;

    void render_phone_to_ui() const;
    zinpass::models::MobilePhone get_phone() const;
    void fetch_phone_by_id(int id) const;

signals:
    void form_submitted();

private slots:
    void on_btn_submit_clicked();
    void on_btn_delete_clicked();

private:
    void setup_ui();
    void initial_input_widgets() const;
    void list_telecom_operators() const;
    void set_input_read_only(bool enable) const;

    QLineEdit* edit_id_;            // disable always
    QComboBox* combo_telecom_;
    QLineEdit* edit_phone_area_;
    QLineEdit* edit_phone_;

    QLineEdit* edit_pin_;
    QLineEdit* edit_puk_;
    QLineEdit* edit_service_pwd_;
    QDateTimeEdit* edit_join_time_;

    QLineEdit* edit_postscript_;
    QLineEdit* edit_user_id_;
    QTextEdit* edit_other_;         // read_only always

    QPushButton* btn_refresh_;
    QPushButton* btn_edit_;
    QPushButton* btn_submit_;
    QPushButton* btn_delete_;

    QHBoxLayout* lyt_main_;
    QFormLayout* lyt_form1_;
    QFormLayout* lyt_form2_;
    QFormLayout* lyt_form3_;
    QVBoxLayout* lyt_btn_;

    zinpass::models::MobilePhone* phone_;
};

#endif // PHONE_DETAIL_FRAME_H
