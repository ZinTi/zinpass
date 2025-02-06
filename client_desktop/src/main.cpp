#include <QApplication>
#include <QDir>
#include <QMessageBox>

#include "state/state_manager.h"
#include "state/channel_manager.h"
#include "user/login_dlg.h"
#include "main_workbench.h"
#include "common/font_util.h"
#include "state/client_config.h"

using namespace zinpass;

void init();    // 初始化

int main(int argc, char *argv[]){
    init();

    QApplication app(argc, argv);

    // 预加载应用程序字体（必须在创建GUI前调用）
    FontUtil::loadApplicationFonts();

    auto* login_dlg = new LoginDlg(nullptr);


    while (true) {
        if (QDialog::Accepted != login_dlg->exec()) {
            break;  // 用户取消登录，退出循环
        }
        auto* main_workbench = new MainWorkbench(nullptr);
        main_workbench->setWindowTitle("工作台 | ZinPass");
        // main_workbench->setStyleSheet("background-color: #8b008b;");

        QObject::connect(main_workbench, &MainWorkbench::sig_btn_clicked, [&](const int index){
            if (index == 1) { // 重新登录
                main_workbench->close();  // 关闭工作台
                // 这里需要调用方法清空数据，避免切换账号后泄露上一用户数据（暂未实现）
                app.exit(-1);
            } else if (index == 2) { // 退出
                // app.quit();
                app.exit(0);
            }
        });
        main_workbench->show();

        const int ret_code = app.exec();  // 进入事件循环，等待用户登录或退出
        if (0 == ret_code) { // 仅当事件循环返回0时，才表示用户退出登录，其他情况循环登录
            break;
        } else if (-1 == ret_code) {
            delete main_workbench;
        }
    }

    delete login_dlg;
    return 0;
}

void init() {
    // 1. 读取配置文件
    std::string config_msg = "";
    state::ClientConfig::instance().read_config_from_file(config_msg);
    // save to log file ......

    // 2. 设置 state 管理器
    const QString executable_path = QCoreApplication::applicationDirPath(); // 检测客户端根路径（依据当前程序所在目录的父目录）
    if (QDir dir(executable_path); dir.cdUp()) {
        const std::string client_root_path = dir.absolutePath().toStdString();
        state::StateManager::instance().set_client_root_path(client_root_path);    // 设置当前客户端路径
    } else {
        // "Failed to get parent directory";
    }

    // 3. 初始化 RPC 连接
    const std::string rpc_server_ap = state::ClientConfig::instance().get_server_ipv4() + ":" + std::to_string(state::ClientConfig::instance().get_server_port());
    zinpass::state::ChannelManager::instance().initialize(rpc_server_ap);
}
