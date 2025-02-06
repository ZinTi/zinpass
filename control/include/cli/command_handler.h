#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <regex>
#include <string>

namespace zinpass::cli {

class CommandHandler {
public:
    int handle(const std::string& rawCommand);

private:
    int execute() const;
    void printError() const;

    std::string raw_command_;

};

} // zinpass::cli

#endif
