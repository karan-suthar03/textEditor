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

        // m_gapBuffer = GapBuffer();
        m_editor = Editor();

        result = 0;
        break;
    }
    case WM_KEYDOWN:{
        result = 0;
        break;
    }
    case WM_SIZE:
        InvalidateRect(m_hwnd, nullptr, TRUE);
        result = 0;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        RECT rc;

        GetClientRect(m_hwnd, &rc);
        
        FillRect(hdc, &rc, m_background);

        HFONT oldFont = (HFONT)SelectObject(hdc, m_font);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, m_fontColor);

        TEXTMETRIC tm;
        GetTextMetricsA(hdc,&tm);

        int windowHeight = rc.bottom - rc.top;
        int windowWidth = rc.right - rc.left;

        int charHeight = tm.tmHeight;
        int charWidth = tm.tmAveCharWidth;

        int lineHeight = charHeight + tm.tmExternalLeading;

        int rows = windowHeight/lineHeight;
        int cols = windowWidth/charWidth;
        
        for (size_t row = 0; row < rows; row++)
        {
            const std::string data = m_editor.getRow(row,cols);
            TextOutA(
                hdc,
                0,
                row * lineHeight,
                data.c_str(),
                static_cast<int>(data.size())
            );
        }

        int cursorCol;
        int cursorRow;
        m_editor.getCursorPosition(cursorRow,cursorCol);

        HPEN oldPen = (HPEN)SelectObject(hdc, m_cursorColor);

        Rectangle(
            hdc,
            cursorCol * charWidth,
            cursorRow * lineHeight,
            cursorCol * charWidth + 3,
            cursorRow * lineHeight + lineHeight
        );

        SelectObject(hdc, oldPen);
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