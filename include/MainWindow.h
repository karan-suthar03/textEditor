#pragma once
#include <windows.h>
#include "GapBuffer.h"


class MainWindow {
public:
    MainWindow();

    bool Create(
        HINSTANCE hInstance,
        const char* className,
        const char* windowTitle,
        int nCmdShow
    );

    HWND Handle() const { return m_hwnd; }

private:
    static LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
    );

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    
    HBRUSH   m_background;
    COLORREF m_fontColor;
    COLORREF m_selectionColor;
    HFONT    m_font;
    HPEN     m_cursorColor;
    
    GapBuffer m_gapBuffer;
};
