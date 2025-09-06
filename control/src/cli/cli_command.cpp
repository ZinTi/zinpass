#include "cli/cli_command.h"
#include <cstdlib>
#include <iostream>
#include "state/channel_manager.h"
#include "rpc/status_rpc.h"
#include <cli/prompt.h>

#define TEMPORARY_CONTROL_KEY "test-key"

namespace zinpass::cli{

void CliCommand::clear(){
#ifdef _WIN32
    std::system("cls");      // Windows 使用 cls
#else
    std::system("clear");    // Linux/Mac 使用 clear
#endif
}

void CliCommand::copyleft() {
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    const rpc::StatusRPC status_rpc(channel);
    const std::string copyleft = status_rpc.get_copyleft(TEMPORARY_CONTROL_KEY);
    std::cout << copyleft << std::endl;
}

void CliCommand::exit(){
    // 通知其他子线程退出
    // 离开
    std::cout << "Bye!" << std::endl;
    std::exit(0);
}
void CliCommand::help(){
    const std::string manual =
        "[ MANUAL ]\n"
        "clear        " "Clear the screen.\n"
        "cls          " "Clear the screen. Same as `cls`.\n"
        "copyleft     " "Print the copyleft.\n"
        "copyright    " "Print the copyleft. Same as `copyleft`.\n"
        "exit         " "Exit zinpassctl.\n"
        "help         " "Print the manual.\n"
        "manual       " "Print the manual. Same as `help`.\n"
        "prompt [new] " "Change your prompt.\n"
        "quit         " "Quit zinpassctl. Same as `exit`.\n"
        "reload       " "reload the configuration.\n"
        "restart      " "restart the ZinPassSvc(zinpassd).\n"
        "start        " "start the ZinPassSvc(zinpassd).\n"
        "status       " "Print status.\n"
        "stop         " "Stop the ZinPassSvc(zinpassd).\n"
        "shutdown     " "Shutdown the ZinPassSvc(zinpassd). Same as `stop`.\n"
        "version      " "Print the version.\n"
    ;
    std::cout << manual << std::endl;
}

void CliCommand::prompt(const std::string& str){
    Prompt::getInstance().setPrompt(str);
}

void CliCommand::reload(){
    std::cout << "当前功能尚未实现" << std::endl;
}
void CliCommand::restart(){
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    const rpc::StatusRPC status_rpc(channel);
    const auto[result_restart, message_restart] = status_rpc.restart(TEMPORARY_CONTROL_KEY);
    std::cout << message_restart << std::endl;
}
void CliCommand::start(){
    const std::string tips = "请手动启动 ZinPassSvc(或 zinpassd)";
    std::cout << tips << std::endl;
}

void CliCommand::status(){
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    const rpc::StatusRPC status_rpc(channel);
    const std::string executable_path = status_rpc.get_executable_path(TEMPORARY_CONTROL_KEY);
    const std::string database_path = status_rpc.get_database_path(TEMPORARY_CONTROL_KEY);
    std::cout << "Executable path: " << executable_path << std::endl;
    std::cout << "Database path: " << database_path << std::endl;
}
void CliCommand::stop(){
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    const rpc::StatusRPC status_rpc(channel);
    const auto[result_stop, message_stop] = status_rpc.shutdown(TEMPORARY_CONTROL_KEY);
    std::cout << message_stop << std::endl;
}
void CliCommand::version(const bool verbose){
    const auto channel = zinpass::state::ChannelManager::get_instance().get_channel();
    const rpc::StatusRPC status_rpc(channel);
    std::string version;
    if (verbose) {
        version = status_rpc.get_version_build_info(TEMPORARY_CONTROL_KEY);
    }else {
        version = status_rpc.get_version(TEMPORARY_CONTROL_KEY);
    }
    std::cout << version << std::endl;
}

}