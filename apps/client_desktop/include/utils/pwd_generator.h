#ifndef PWD_GENERATOR_H
#define PWD_GENERATOR_H

#include <cstdint>
#include <string>
#include <vector>
#include <atomic>
#include <random>

class PwdGenerator {
    mutable std::mt19937 generator;
    static std::atomic<unsigned> seed_counter;

    uint8_t components_;
    unsigned int length_; // 长度
    unsigned int count_; // 数量

    /**
     * @brief 生成一个随机字符串
     * @return 返回生成的密码
     */
    std::string GenerateRandomString() const;

    /**
     * @brief 验证生成的密码是否合法: 密码长度是否符合要求，恰好包含且仅包含用户要求的字符类型
     * @param passwd 密码
     * @return
     */
    bool isValidString(const std::string& passwd) const;

public:
    /**
     * @brief 构造函数
     * @param length 密码长度，范围 [6, 256]
     * @param count 密码数量，范围 [1, 100]
     */
    PwdGenerator(unsigned int length, unsigned int count);
    ~PwdGenerator();

    /**
     * @brief 设置密码组成字符类型
     * @param components 00000001 代表包含数字，00000010 代表包含大写字母，00000100 代表包含小写字母，00001000 代表包含符号
     * 合法性范围 [1, 15]
     */
    void setComponents(uint8_t components);

    /**
     * @brief 设置密码组成字符类型
     * @param has_digit 包含数字
     * @param has_upper 包含大写字母
     * @param has_lower 包含小写字母
     * @param has_symbol 包含特殊字符
     */
    void setComponentsByBool(bool has_digit, bool has_upper, bool has_lower, bool has_symbol);

    /**
     * @brief 获取密码
     * @param passwords 密码列表
     * @return 成功与否
     */
    bool getPasswords(std::vector<std::string>& passwords) const;
};

#endif
