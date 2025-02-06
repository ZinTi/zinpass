#ifndef PROMPT_H
#define PROMPT_H

#include <mutex>
#include <string>

namespace zinpass::cli {

class Prompt {
public:
    // 删除拷贝构造和赋值运算符
    Prompt(const Prompt&) = delete;
    Prompt& operator=(const Prompt&) = delete;

    // 获取单例实例
    static Prompt& getInstance() {
        static Prompt instance;
        return instance;
    }

    // === 成员函数 ===
    std::string getPrompt();
    void setPrompt(const std::string& prompt);
    std::string getUsername();
    void setUsername(const std::string& username);

    void print() const;

private:
    Prompt(); // 只保留默认构造函数
    ~Prompt() = default;

    // === 成员变量 ===
    std::string username_;
    std::string prompt_;

    mutable std::mutex mutex_;  // 线程安全互斥锁
};

} // namespace zinpass::cli

#endif // PROMPT_H