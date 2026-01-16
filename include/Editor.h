#pragma once
#include <string>
#include <vector>
#include "GapBuffer.h"

class Editor {
public:
    Editor();

    void loadFromString(const std::string& text);
    void getCursorPosition(int& row, int& col) const;

    std::string getRow(int row,int size = 100) const;

private:
    GapBuffer m_buffer;
    std::vector<size_t> m_lineStarts;

};
