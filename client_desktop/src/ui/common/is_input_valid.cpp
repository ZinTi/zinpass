#include "common/is_input_valid.h"
#include <QMessageBox>
#include <QRegularExpression>

// 用户注册界面输入数据合法性校验
bool isPasswordValid(const QString& password1, const QString& password2) {
    if (password1!= password2) {
        QMessageBox::warning(nullptr, QString("密码不合法"), QString("两次输入的密码不相同，请重新输入！"));
        return false;
    }
    const QRegularExpression regex("^(?=.*[0-9])(?=.*[a-zA-Z])(?=.*[^a-zA-Z0-9]).{8,50}$");
    const QRegularExpressionMatch match = regex.match(password1);
    if (!match.hasMatch()) {
        QMessageBox::warning(nullptr, QString("密码不合法"), QString("密码不合法，请输入 8-50 位由数字、字母、其他字符组成，且必须包含 3 种类型的字符，不能只包含 1 种或 2 种的密码！"));
        return false;
    }
    return true;
}
bool isUsernameValid(const QString& username) {
    const QRegularExpression regex("^[a-z_][a-z0-9_]{2,15}$");
    const QRegularExpressionMatch match = regex.match(username);
    if (!match.hasMatch()) {
        QMessageBox::warning(nullptr, QString("用户名不合法"), QString("用户名不合法，请输入 3-16 位由小写英文字母、数字、下划线组成，且不能以数字为首的用户名！"));
        return false;
    }
    return true;
}
bool isNicknameValid(const QString& nickname) {
    const QRegularExpression regex("^.{1,36}$");
    const QRegularExpressionMatch match = regex.match(nickname);
    if (!match.hasMatch()) {
        QMessageBox::warning(nullptr, QString("昵称不合法"), QString("昵称不合法，请输入长度为 1-36 字节的昵称！"));
        return false;
    }
    return true;
}

// 主工作台界面
bool isUTimeRangeValid(const QDateTime& dateRangeStart, const QDateTime& dateRangeEnd) {
    // dateRangeStart 不能晚于 dateRangeEnd
    if (dateRangeStart.isValid() && dateRangeEnd.isValid() && dateRangeStart.secsTo(dateRangeEnd) < 0) {
        QMessageBox::warning(nullptr, QString("时间范围不合法"), QString("开始时间不能晚于结束时间，请重新输入！"));
        return false;
    }
    return true;
}
