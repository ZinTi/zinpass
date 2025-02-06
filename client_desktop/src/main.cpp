#include <QApplication>
#include <QDir>
#include <QMessageBox>

#include "state_manager.h"
#include "login_dlg.h"
#include "main_workbench.h"
#include "common/font_util.h"

using namespace zinpass;

void init();    // 初始化

int main(int argc, char *argv[]){
    init();

    QApplication app(argc, argv);

    // 预加载应用程序字体（必须在创建GUI前调用）
    FontUtil::loadApplicationFonts();

    auto* login_dlg = new LoginDlg(nullptr);
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

    while (true) {
        if (QDialog::Accepted != login_dlg->exec()) {
            break;  // 用户取消登录，退出循环
        }

        main_workbench->show();

        if (0 == app.exec()) { // 仅当事件循环返回0时，才表示用户退出登录，其他情况循环登录
            break;
        }
    }

    delete main_workbench;
    delete login_dlg;
    return 0;
}

void init() {
    // 检测客户端根路径（依据当前程序所在目录的父目录）
    const QString executablePath = QCoreApplication::applicationDirPath();
    if (QDir dir(executablePath); dir.cdUp()) {
        const std::string clientRootPath = dir.absolutePath().toStdString();
        auto& state = state::StateManager::instance();
        state.setClientRootPath(clientRootPath);    // 设置当前客户端路径
    } else {
        // "Failed to get parent directory";
    }
}
