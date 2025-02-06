#include "utils/pwd_generator.h"
#include <random>
#include <chrono>

namespace zinpass::utils{

PwdGenerator::PwdGenerator(PwdComponentsType components, int length, int count){
    this->components = components;
    this->length = length;
    this->count = count;
}
PwdGenerator::~PwdGenerator()= default;

bool PwdGenerator::getGwd(std::vector<std::string>& passwords) const {
    for (int i = 0; i < this->count; i++) {  // this->count 密码数量
        passwords.push_back(GenerateRandomPwd());
    }
    return true;
}

// 根据 this->components 生成相应成分的密码, 密码字符串长度为this->length
std::string PwdGenerator::GenerateRandomPwd() const {
    std::string availableChars;
    // 根据 components 的标志位添加可用字符集
    if (components.getDigits()) {
        availableChars += DIGITS;
    }
    if (components.getUppercase()) {
        availableChars += UPPERCASE;
    }
    if (components.getLowercase()) {
        availableChars += LOWERCASE;
    }
    if (components.getSymbols()) {
        availableChars += SYMBOLS;
    }
    // 如果可用字符集为空，返回空字符串
    if (availableChars.empty()) {
        return "";
    }

    // 使用当前时间作为随机数生成器的种子
    const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<> distribution(0, static_cast<int>(availableChars.length()) - 1);

    std::string password;
    for (int i = 0; i < length; ++i) {
        // 从可用字符集中随机选择一个字符
        const int randomIndex = distribution(generator);
        password += availableChars[randomIndex];
    }

    return password;
}

} // zinpass::utils
