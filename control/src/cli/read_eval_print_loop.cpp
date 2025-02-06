#include "read_eval_print_loop.h"
#include <iostream>
#include "command_handler.h"

namespace zinpass::cli {
    ReadEvalPrintLoop::ReadEvalPrintLoop() {
        this->command_handler_ = new CommandHandler();
    }

    ReadEvalPrintLoop::~ReadEvalPrintLoop() {
        delete this->command_handler_;
    }

    void ReadEvalPrintLoop::exit() { this->quit_ = true; }

    int ReadEvalPrintLoop::run() {
        while (!this->quit_) {
            Prompt::getInstance().print();
            std::getline(std::cin, input_);
            command_handler_->handle(input_);

        }
        return 0;
    }
}