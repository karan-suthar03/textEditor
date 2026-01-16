#include "../include/GapBuffer.h"

GapBuffer::GapBuffer(size_t initial_capacity)
    : buffer(initial_capacity, '\0'),
      gapStart(0),
      gapEnd(initial_capacity) {
}

size_t GapBuffer::cursor() const {
    return gapStart;
}

void GapBuffer::insert(char c) {
    buffer[gapStart++] = c;
}

void GapBuffer::backspace() {
    if (gapStart > 0) {
        gapStart--;
    }
}

void GapBuffer::moveLeft() {
    if (gapStart == 0) return;

    buffer[--gapEnd] = buffer[--gapStart];
}

void GapBuffer::moveRight() {
    if (gapEnd == buffer.size()) return;

    buffer[gapStart++] = buffer[gapEnd++];
}

void GapBuffer::loadFromString(const std::string& text) {
    size_t gapSize = 1024;

    buffer.resize(text.size() + gapSize);

    gapStart = 0;
    gapEnd   = gapSize - 1;

    memcpy(
        buffer.data() + gapEnd + 1,
        text.data(),
        text.size()
    );
}

std::string GapBuffer::toString() const {
    std::string result;
    result.reserve(buffer.size() - (gapEnd - gapStart + 1));

    result.append(buffer.data(), gapStart);
    result.append(buffer.data() + gapEnd + 1, buffer.size() - gapEnd - 1);

    return result;
}

std::string GapBuffer::getLineFrom(size_t startIndex, size_t size) const {
    std::string result;
    result.reserve(size);

    size_t logicalIndex = startIndex;
    size_t endIndex = startIndex + size;

    while (logicalIndex < endIndex) {
        char c;

        

        size_t physicalIndex;
        if (logicalIndex < gapStart) {
            physicalIndex = logicalIndex;
        } else {
            physicalIndex = logicalIndex + (gapEnd - gapStart);
        }
        
        if (physicalIndex >= buffer.size()) {
            break;
        }
        
        c = buffer[physicalIndex];
        
        if (c == '\n') {
            break;
        }
        
        result += c;
        logicalIndex++;
    }
    return result;
}