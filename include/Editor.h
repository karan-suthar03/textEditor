#pragma once
#include <string>
#include <vector>
#include "GapBuffer.h"

class Editor {
public:
    GapBuffer m_buffer;

    Editor(const std::string& text = "");

    void loadFromString(const std::string& text);
    void getCursorPosition(int& row, int& col) const;

    bool handleCharacterInput(char c);

    std::string getRow(int row,int size = 100) const;

    // use enum and make these private later
    void moveUp();
    void moveDown();
    void backspace();
    void newline();
private:
    std::vector<size_t> m_lineStarts;
};
