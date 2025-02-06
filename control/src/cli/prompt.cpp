#include "prompt.h"
#include <iostream>

#define USERNAME_DEFAULT "root"
#define PROMPT_DEFAULT "$ "

namespace zinpass::cli {

// 只保留默认构造函数
Prompt::Prompt()
    : username_(USERNAME_DEFAULT), prompt_(PROMPT_DEFAULT) {}

// 线程安全的成员函数实现
std::string Prompt::getPrompt() {
    std::lock_guard<std::mutex> lock(mutex_);
    return prompt_;
}

void Prompt::setPrompt(const std::string& prompt) {
    std::lock_guard<std::mutex> lock(mutex_);
    prompt_ = prompt;
}

std::string Prompt::getUsername() {
    std::lock_guard<std::mutex> lock(mutex_);
    return username_;
}

void Prompt::setUsername(const std::string& username) {
    std::lock_guard<std::mutex> lock(mutex_);
    username_ = username;
}

void Prompt::print() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "[" << username_ << "]" << std::endl;
    std::cout << prompt_;
}

} // namespace zinpass::cli