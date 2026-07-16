#include "application.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "utils/tmp_msg.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
){
    tmp_msg_add("Starting service...");
    Application app;
    app.init();
    app.run();

    return 0;
}
