/**
 * ZinPassCtl 通过 gRPC 127.0.0.1:50052 与 ZinPassSvc 交互

  zinpassctl status  # 查看服务状态
  zinpassctl start   # 启动服务
  zinpassctl stop    # 停止服务
  zinpassctl restart # 重新启动
  zinpassctl reload  # 重载配置
  zinpassctl version # 查看版本

  zinpassctl         # 进入 REPL 交互模式

 */
#include "read_eval_print_loop.h"
#include "cli_command.h"
#include <iostream>
#if defined(_WIN32)
    #include "windows.h"
#endif

using namespace zinpass;

int main(int argc, char* argv[]){
#if defined(_WIN32)
    SetConsoleCP(65001) && SetConsoleOutputCP(65001);
#endif

    cli::CliCommand::version(true);
    std::cout << "Enter \"help\" for usage hints." << std::endl;

    // 获取单例实例
    auto& prompt = cli::Prompt::getInstance();

    cli::ReadEvalPrintLoop REPL;
    REPL.run();
    return 0;
}