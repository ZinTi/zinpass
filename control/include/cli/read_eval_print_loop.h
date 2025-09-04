#ifndef READ_EVAL_PRINT_LOOP_H
#define READ_EVAL_PRINT_LOOP_H
#include "cli/command_handler.h"
#include "cli/prompt.h"

namespace zinpass::cli {
    class ReadEvalPrintLoop {
    public:
        ReadEvalPrintLoop();

        ~ReadEvalPrintLoop();

        // 离开交互
        void exit();

        // 运行 REPL 交互
        int run();

    private:
        unsigned long command_count_ = 0; // 用户输入指令 计数器
        bool quit_ = false;

        std::string input_;
        CommandHandler* command_handler_;
};
}

#endif
