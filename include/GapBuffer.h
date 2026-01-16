#pragma once
#include <vector>
#include <string>

class GapBuffer {
public:
    explicit GapBuffer(size_t initial_capacity = 1024);

    void moveLeft();
    void moveRight();

    void insert(char c);
    void backspace();

    void loadFromString(const std::string& str);
    std::string toString() const;

    size_t cursor() const;

private:
    std::vector<char> buffer;
    size_t gapStart;
    size_t gapEnd;
};