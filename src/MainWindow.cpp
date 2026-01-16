#include "../include/MainWindow.h"
#include <cstdio>

MainWindow::MainWindow()
    : m_hwnd(nullptr),
      m_background(nullptr),
      m_fontColor(RGB(208, 208, 208)),
      m_selectionColor(RGB(50, 50, 255)),
      m_font(nullptr),
      m_cursorColor(nullptr) {
}
bool MainWindow::Create(
    HINSTANCE hInstance,
    const char* className,
    const char* windowTitle,
    int nCmdShow) {

    WNDCLASS wc = {};
    wc.lpfnWndProc = MainWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;

    if (!RegisterClass(&wc)) {
        return false;
    }

    m_hwnd = CreateWindowExA(
        0,
        className,
        windowTitle,
        WS_OVERLAPPEDWINDOW | WS_CAPTION,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    if (m_hwnd == nullptr) {
        return false;
    }

    ShowWindow(m_hwnd, nCmdShow);
    UpdateWindow(m_hwnd);
    return true;
}

LRESULT CALLBACK MainWindow::WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {

    MainWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->m_hwnd = hwnd;
        return TRUE;
    } else {
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessage(uMsg, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT MainWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {

    case WM_CREATE:
    {
        m_background = CreateSolidBrush(RGB(18, 18, 18));
        m_fontColor = RGB(208, 208, 208);
        m_selectionColor = RGB(50, 50, 255);

        m_font = CreateFont(
            -24, 0, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            "Consolas"
        );

        m_cursorColor = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

        result = 0;
        break;
    }
    case WM_SIZE:
        result = 0;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, m_background);

        HFONT oldFont = (HFONT)SelectObject(hdc, m_font);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, m_fontColor);

        const char* demoText =
            "Hello Gap Buffer!\n"
            "This is a demo.\n"
            "Rendering text using WinAPI.\n";

        int x = 0;
        int y = 0;

        const char* start = demoText;
        const char* p = demoText;

        while (*p) {
            if (*p == '\n') {
                TextOutA(hdc, x, y, start, (int)(p - start));
                y += 24;
                start = p + 1;
            }
            p++;
        }

        if (p != start) {
            TextOutA(hdc, x, y, start, (int)(p - start));
        }

        SelectObject(hdc, oldFont);

        EndPaint(m_hwnd, &ps);
        result = 0;
        break;
    }

    case WM_DESTROY:
        if (m_background) DeleteObject(m_background);
        if (m_font) DeleteObject(m_font);
        if (m_cursorColor) DeleteObject(m_cursorColor);

        PostQuitMessage(0);
        result = 0;
        break;

    case WM_ERASEBKGND:
        result = 1;
        break;

    default:
        result = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        break;
    }

    return result;
}