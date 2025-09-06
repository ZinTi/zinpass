#include "cli/command_handler.h"

#include <iostream>
#include "cli/cli_command.h"

namespace zinpass::cli {
    int CommandHandler::handle(const std::string& rawCommand) {
        this->raw_command_ = rawCommand;

        if(raw_command_ == "clear" || raw_command_ == "cls"){
            CliCommand::clear();
        } else if (raw_command_ == "copyleft" || raw_command_ == "copyright") {
            CliCommand::copyleft();
        } else if(raw_command_ == "exit" || raw_command_ == "quit"){
            CliCommand::exit();
        } else if(raw_command_ == "help" || raw_command_ == "manual"){
            CliCommand::help();
        } else if(raw_command_ == "prompt") {
            CliCommand::prompt("");
        } else if (0 == raw_command_.compare(0, 7, "prompt ")) {
            CliCommand::prompt(raw_command_.substr(7));
        } else if(raw_command_ == "reload") {
            CliCommand::reload();
        } else if(raw_command_ == "restart") {
            CliCommand::restart();
        } else if(raw_command_ == "start") {
            CliCommand::start();
        } else if(raw_command_ == "status") {
            CliCommand::status();
        } else if(raw_command_ == "stop" || raw_command_ == "shutdown") {
            CliCommand::stop();
        } else if(raw_command_ == "version"){
            CliCommand::version(false);
        } else {
            printError();
            return -1;
        }

        return 0;
    }

    int CommandHandler::execute() const {
        std::cout << this->raw_command_ << std::endl;
        return 0;
    }

    void CommandHandler::printError() const {
        std::cout << this->raw_command_ << ": command not found" << std::endl;
    }

} // zinpass::cli