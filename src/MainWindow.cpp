#include "../include/MainWindow.h"
#include "../include/debug.h"
#include <cstdio>
#include "../include/base.h"

MainWindow::MainWindow()
    : m_hwnd(nullptr),
      m_background(nullptr),
      m_fontColor(RGB(208, 208, 208)),
      m_selectionColor(RGB(50, 50, 255)),
      m_font(nullptr),
      m_cursorColor(nullptr),
      m_scrollTopLine(0),
      m_scrollLeftCol(0),
      m_openedFilePath("") {
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
    DragAcceptFiles(m_hwnd, TRUE);
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
        m_editor = Editor("\r\nhello\nworld\nthis is a test");

        result = 0;
        break;
    }
    case WM_KEYDOWN:{
        bool r = false;
        bool ctrlDown = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        switch(wParam){
            case VK_LEFT:
                m_editor.m_buffer.moveLeft();
                r = true;
                break;
            case VK_RIGHT:
                m_editor.m_buffer.moveRight();
                r = true;
                break;
            case VK_BACK:
                m_editor.backspace();
                r = true;
                break;
            case VK_UP:
                m_editor.moveUp();
                r = true;
                break;
            case VK_DOWN:
                m_editor.moveDown();
                r = true;
                break;
            case VK_RETURN:
                m_editor.newline();
                r = true;
                break;
            case 'S':
                if(ctrlDown){
                    saveFile();
                }
                r = true;
                break;
            default:
                break;
        }
        if(r) {
            adjustScrollToCursor();
            InvalidateRect(m_hwnd, nullptr, TRUE);
        }
        result = 0;
        break;
    }
    case WM_CHAR:{
        char c = static_cast<char>(wParam);        
        bool ctrlDown = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        if(c >= 32 && !ctrlDown){
            if(m_editor.handleCharacterInput(c)) InvalidateRect(m_hwnd, nullptr, TRUE);
        }
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

        int rows = windowHeight/lineHeight + 1;
        int cols = windowWidth/charWidth + 1;
        
        for (size_t row = 0; row < rows; row++)
        {
            std::string visiblePart = m_editor.getRow(m_scrollTopLine + row, cols, m_scrollLeftCol);
            TextOutA(
                hdc,
                0,
                row * lineHeight,
                visiblePart.c_str(),
                static_cast<int>(visiblePart.size())
            );
        }

        int cursorCol;
        int cursorRow;
        m_editor.getCursorPosition(cursorRow,cursorCol);

        HPEN oldPen = (HPEN)SelectObject(hdc, m_cursorColor);

        Rectangle(
            hdc,
            (cursorCol - m_scrollLeftCol) * charWidth,
            (cursorRow - m_scrollTopLine) * lineHeight,
            (cursorCol - m_scrollLeftCol) * charWidth + 3,
            (cursorRow - m_scrollTopLine) * lineHeight + lineHeight
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

    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;
        int file_count = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
        LOG("Files dropped: %d\n", file_count);
        if (file_count == 1) {
            char file_path[KB(4)];
            DragQueryFileA(hDrop, 0, file_path, KB(4));
            LOG("Loading file: %s\n", file_path);

            loadFile(file_path);
            InvalidateRect(m_hwnd, nullptr, TRUE);
        }
        result = 0;
        break;
    }
    default:
        result = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        break;
    }

    return result;
}

void MainWindow::loadFile(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        std::string fileContent;
        fileContent.resize(fileSize);

        fread(&fileContent[0], 1, fileSize, file);
        fclose(file);
        m_openedFilePath = filePath;
        
        m_editor = Editor(fileContent);
        LOG("File loaded successfully. Size: %ld bytes\n", fileSize);
    } else {
        LOG("Failed to open file: %s\n", filePath);
    }
}

void MainWindow::adjustScrollToCursor() {
    TEXTMETRIC tm;
    RECT rc;
    
    HDC hdc = GetDC(m_hwnd);
    HFONT oldFont = (HFONT)SelectObject(hdc, m_font);

    GetClientRect(m_hwnd, &rc);
    GetTextMetricsA(hdc,&tm);
    
    int windowHeight = rc.bottom - rc.top;
    int windowWidth = rc.right - rc.left;

    int charHeight = tm.tmHeight;
    int charWidth = tm.tmAveCharWidth;

    int rows = windowHeight/charHeight;
    int cols = windowWidth/charWidth;
    
    SelectObject(hdc, oldFont);
    ReleaseDC(m_hwnd, hdc);

    int cursorRow, cursorCol;
    m_editor.getCursorPosition(cursorRow, cursorCol);
    
    if (cursorRow < m_scrollTopLine) {
        m_scrollTopLine = cursorRow;
    } else if (cursorRow >= m_scrollTopLine + rows) {
        m_scrollTopLine = cursorRow - rows + 1;
    }
    
    if (cursorCol < m_scrollLeftCol) {
        m_scrollLeftCol = cursorCol;
    } else if (cursorCol >= m_scrollLeftCol + cols) {
        m_scrollLeftCol = cursorCol - cols + 1;
    }
    
    if (m_scrollTopLine < 0) m_scrollTopLine = 0;
    if (m_scrollLeftCol < 0) m_scrollLeftCol = 0;
}

void MainWindow::saveFile() {
    if(m_openedFilePath.empty()){
        return;
    }
    FILE* file = fopen(m_openedFilePath.c_str(), "wb");
    if (file) {
        std::string content = m_editor.m_buffer.toString();
        fwrite(content.c_str(), 1, content.size(), file);
        fclose(file);
        LOG("File saved successfully: %s\n", m_openedFilePath.c_str());
    } else {
        LOG("Failed to open file for saving: %s\n", m_openedFilePath.c_str());
    }
}