#include "../include/MainWindow.h"

int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow) {

    MainWindow mainWindow;

    if (!mainWindow.Create(hInstance, "MyWindowClass", "Text editor", nCmdShow)) {
        return -1;
    }

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}