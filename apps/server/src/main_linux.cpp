#include "main_linux.h"
#include "application.h"

#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <csignal>
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <string>
#include "utils/tmp_msg.h"

int main(){
    tmp_msg_add("Starting daemon...");
    daemonize();

    // 初始化
    Application app;
    app.init();
    app.run();

    return 0;
}

void daemonize() {
    // 第一次 fork
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE); // fork 失败
    if (pid > 0) exit(EXIT_SUCCESS); // 第一父进程退出

    if (setsid() < 0) exit(EXIT_FAILURE); // 成为新会话组长，脱离终端

    signal(SIGHUP, SIG_IGN); // 忽略 SIGHUP 信号（可选，部分系统在会话首进程退出时会发）

    // 第二次 fork
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS); // 第二父进程退出

    umask(0); // 设置文件创建掩码

    // 变更工作目录
    if (chdir("/") != 0) {
        std::string msg = "chdir / failed: " + std::string(strerror(errno));
        tmp_msg_add(msg);
    }

    // 关闭并重定向标准文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    int fd = open("/dev/null", O_RDONLY);
    if (fd != STDIN_FILENO) { // 应该等于 0
        std::string msg = "Failed to redirect stdin to /dev/null: " + std::string(strerror(errno));
        tmp_msg_add(msg);
        exit(EXIT_FAILURE);
    }
    fd = open("/dev/null", O_RDWR);
    if (fd != STDOUT_FILENO) { // 应该等于 1
        std::string msg = "Failed to redirect stdout to /dev/null: " + std::string(strerror(errno));
        tmp_msg_add(msg);
        exit(EXIT_FAILURE);
    }
    // 将 stderr 也重定向到同一个 /dev/null，或重定向到日志文件
    if (dup2(STDOUT_FILENO, STDERR_FILENO) == -1) {
        std::string msg = "Failed to redirect stderr: " + std::string(strerror(errno));
        tmp_msg_add(msg);
        exit(EXIT_FAILURE);
    }
    tmp_msg_add("Standard file descriptors redirected, daemon initialization successful");
}
