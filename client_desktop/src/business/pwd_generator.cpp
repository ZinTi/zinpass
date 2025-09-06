#include "business/pwd_generator.h"
#include <stdexcept>

// 定义密码组成成分，在此定义避免污染全局命名空间
#define DIGITS           "0123456789"
#define UPPERCASE        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE        "abcdefghijklmnopqrstuvwxyz"
#define SYMBOLS          "!@#$%^&*()_+{}[]|:;<>,.?/~"

// 初始化随机数生成器的种子
std::atomic<unsigned> PwdGenerator::seed_counter(0);

PwdGenerator::PwdGenerator(const unsigned int length, const unsigned int count)
    : length_(length), count_(count) {
    components_ = 15; // 默认包含所有字符类型

    // 用真随机数初始化引擎
    std::random_device rd;
    generator.seed(rd() ^ seed_counter.fetch_add(1));
}
PwdGenerator::~PwdGenerator()= default;

void PwdGenerator::setComponents(const uint8_t components){
    components_ = components;
    if (components_ < 1 || components_ > 15 ||
        length_ < 6 || length_ > 256 ||
        count_ < 1 || count_ > 100) {
        throw std::invalid_argument("Invalid argument");
    }
}

void PwdGenerator::setComponentsByBool(const bool has_digit, const bool has_upper, const bool has_lower, const bool has_symbol) {
    components_ = 0; // 清空 components_
    if (has_digit) components_ |= (1 << 0); // |= 0001
    if (has_upper) components_ |= (1 << 1); // |= 0010
    if (has_lower) components_ |= (1 << 2); // |= 0100
    if (has_symbol) components_ |= (1 << 3); // |= 1000

    if (components_ < 1 || components_ > 15 ||
        length_ < 6 || length_ > 256 ||
        count_ < 1 || count_ > 100) {
        throw std::invalid_argument("Invalid argument");
    }
}

bool PwdGenerator::getPasswords(std::vector<std::string>& passwords) const {
    for (int i = 0; i < count_; i++) {
        int attempts = 0;
        std::string passwd;
        do {
            passwd = GenerateRandomString();
            if (passwd.empty()) return false; // 生成失败
        } while (!isValidString(passwd) && ++attempts < 1000); // 限制最大尝试次数，为提升短密码生成成功率，例如4中字符的长度为6的密码
        if (attempts >= 1000) {
            passwd = "[Exceeding the maximum attempt limit]"; // 超过最大尝试次数，返回一个字符串密码
        }

        passwords.push_back(passwd);
    }
    return true;
}

// 根据 this->components_ 生成相应成分的密码, 密码字符串长度为this->length_
std::string PwdGenerator::GenerateRandomString() const {
    std::string availableChars;
    // 根据 components_ 的标志位添加可用字符集
    if (components_ & (1 << 0)) availableChars += DIGITS; // 按位与判断是否包含数字
    if (components_ & (1 << 1)) availableChars += UPPERCASE; // 按位与判断是否包含大写字母
    if (components_ & (1 << 2)) availableChars += LOWERCASE; // 按位与判断是否包含小写字母
    if (components_ & (1 << 3)) availableChars += SYMBOLS; // 按位与判断是否包含特殊符号

    // 如果可用字符集为空，返回空字符串
    if (availableChars.empty()) {
        return "";
    }

    std::uniform_int_distribution<> distribution(0, static_cast<int>(availableChars.length()) - 1);

    std::string password;
    for (int i = 0; i < length_; ++i) {
        // 从可用字符集中随机选择一个字符
        const int randomIndex = distribution(generator);
        password += availableChars[randomIndex];
    }

    return password;
}

bool PwdGenerator::isValidString(const std::string& passwd) const {
    if (passwd.length() != this->length_) return false;

    // 统计密码是否包含数字、大写字母、小写字母、特殊符号
    uint8_t comp_passwd = 0; // 0001: digits, 0010: uppercase, 0100: lowercase, 1000: special symbols
    for (const char& c : passwd) {
        if ( c >= '0' && c <= '9' ) {
            comp_passwd |= (1 << 0); // |= 0001
        } else if ( c >= 'A' && c <= 'Z' ) {
            comp_passwd |= (1 << 1); // |= 0010
        } else if ( c >= 'a' && c <= 'z' ) {
            comp_passwd |= (1 << 2); // |= 0100
        } else {
            comp_passwd |= (1 << 3); // |= 1000
        }
    }

    // 恰好包含且仅包含用户要求的字符类型
    if (comp_passwd == this->components_) return true;
    return false;
}
