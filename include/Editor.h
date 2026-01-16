#pragma once
#include <string>
#include <vector>
#include "GapBuffer.h"
#include <windows.h>

class Editor {
public:
    Editor();

    void loadFromString(const std::string& text);
    void getCursorPosition(int& row, int& col) const;

    std::string getRow(int row,int size = 100) const;

    bool handleKeyDown(WPARAM key);

private:
    void moveUp();
    void moveDown();
    void backspace();
    void newline();
    
    GapBuffer m_buffer;
    std::vector<size_t> m_lineStarts;

};
