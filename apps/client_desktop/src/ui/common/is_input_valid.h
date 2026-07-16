#ifndef IS_INPUT_VALID_H
#define IS_INPUT_VALID_H

#include <QDialog>
#include <QDateTime>

// 0. 注册用户界面
bool isPasswordValid(const QString& password1, const QString& password2);
bool isUsernameValid(const QString& username);
bool isNicknameValid(const QString& nickname);

// 1. 主工作台界面
// (1.1) 账号记录查询-筛选控件
bool isUTimeRangeValid(const QDateTime& dateRangeStart, const QDateTime& dateRangeEnd);

#endif
