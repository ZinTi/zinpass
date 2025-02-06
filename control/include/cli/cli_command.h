#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H
#include <string>

namespace zinpass::cli{

class CliCommand {
public:
    static void clear();
    static void copyleft();
    static void exit();
    static void help();
    static void prompt(const std::string& str);
    static void reload();
    static void restart();
    static void start();
    static void status();
    static void stop();
    static void version(bool verbose);
};

}


#endif //CLI_COMMAND_H
