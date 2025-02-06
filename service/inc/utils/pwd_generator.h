#ifndef PWD_GENERATOR_H
#define PWD_GENERATOR_H

#include <string>
#include <vector>

// 定义密码组成成分
#define DIGITS           "0123456789"
#define UPPERCASE        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE        "abcdefghijklmnopqrstuvwxyz"
#define SYMBOLS          "!@#$%^&*()_+{}[]|:;<>,.?/~"

namespace zinpass::utils{

typedef class PwdComponentsType{
    bool digits;
    bool uppercase;
    bool lowercase;
    bool symbols;

public:
    explicit PwdComponentsType(const bool digits = true, const bool uppercase = true, const bool lowercase = true, const bool symbols = true)
        : digits(digits), uppercase(uppercase), lowercase(lowercase), symbols(symbols) {}
    PwdComponentsType(const PwdComponentsType& other){
        digits = other.digits;
        uppercase = other.uppercase;
        lowercase = other.lowercase;
        symbols = other.symbols;
    }
    ~PwdComponentsType()= default;
    PwdComponentsType& operator=(const PwdComponentsType& other){
        digits = other.digits;
        uppercase = other.uppercase;
        lowercase = other.lowercase;
        symbols = other.symbols;
        return *this;
    }
    bool operator==(const PwdComponentsType& other) const{
        return digits == other.digits && uppercase == other.uppercase && lowercase == other.lowercase && symbols == other.symbols;
    }
    bool operator!=(const PwdComponentsType& other) const{
        return !(*this == other);
    }
    void setDigits(const bool newDigits){
        digits = newDigits;
    }
    void setUppercase(const bool newUppercase){
        uppercase = newUppercase;
    }
    void setLowercase(const bool newLowercase){
        lowercase = newLowercase;
    }
    void setSymbols(const bool newSymbols){
        symbols = newSymbols;
    }
    [[nodiscard]] bool getDigits() const{
        return digits;
    }
    [[nodiscard]] bool getUppercase() const{
        return uppercase;
    }
    [[nodiscard]] bool getLowercase() const{
        return lowercase;
    }
    [[nodiscard]] bool getSymbols() const{
        return symbols;
    }
} PwdComponentsType;

class PwdGenerator {
    PwdComponentsType components;
    int length; // 长度
    int count; // 数量

    /**
     * @brief 生成一个随机密码
     * @return 返回生成的密码
     */
    std::string GenerateRandomPwd() const;

public:
    PwdGenerator(PwdComponentsType components, int length, int count);
    ~PwdGenerator();
    bool getGwd(std::vector<std::string>& passwords) const;
};

}

#endif
