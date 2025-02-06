#ifndef TMP_MSG_H
#define TMP_MSG_H

#include <string>

/**
 * @brief 是否为空
 * @return 若为空，返回 true；否则返回 false
 */
bool tmp_msg_empty();

/**
 * @brief 新增临时消息
 * @param msg 消息
 */
void tmp_msg_add(const std::string& msg);

/**
 * @brief 获取临时消息
 * @note 若为空，返回空字符串 ""
 * @return 临时消息
 */
std::string tmp_msg_get();

/**
 * @brief 获取所有临时消息
 * @note 返回所有临时消息，拼接成一个字符串
 * @param delimiter 分隔符，默认换行符
 * @return 所有临时消息
 */
std::string tmp_msg_take_all(const std::string& delimiter = "\n");

#endif
