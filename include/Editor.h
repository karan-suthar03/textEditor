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

    bool getRow(int row, std::string& out, int size = 100, int offset = 0) const;

    // use enum and make these private later
    void moveUp();
    void moveDown();
    void backspace();
    void newline();

    size_t getMaxLineNum() const {
        return m_lineStarts.size();
    }
private:
    std::vector<size_t> m_lineStarts;
};
