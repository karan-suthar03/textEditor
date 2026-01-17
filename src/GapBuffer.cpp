#include "../include/GapBuffer.h"
#include "../include/base.h"

GapBuffer::GapBuffer(size_t initial_capacity)
    : buffer(initial_capacity, '\0'),
      gapStart(0),
      gapEnd(initial_capacity) {
}

size_t GapBuffer::cursor() const {
    return gapStart;
}

size_t GapBuffer::size() const {
    return buffer.size() - (gapEnd - gapStart);
}

void GapBuffer::insert(char c) {

    if (gapStart == gapEnd) {
        size_t oldGapSize = gapEnd - gapStart;
        size_t newGapSize = oldGapSize + gapSize;

        buffer.insert(buffer.begin() + gapStart, gapSize, '\0');
        gapEnd += gapSize;
    }    
    buffer[gapStart++] = c;
}

void GapBuffer::deleteLeft() {
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
    buffer.clear();
    buffer.reserve(text.size() + gapSize);
    buffer.insert(buffer.end(), text.begin(), text.end());
    gapStart = text.size();
    gapEnd = buffer.size();
}

std::string GapBuffer::toString() const {
    std::string result;
    result.reserve(buffer.size() - (gapEnd - gapStart + 1));

    result.append(buffer.data(), gapStart);
    result.append(buffer.data() + gapEnd + 1, buffer.size() - gapEnd - 1);

    return result;
}

std::string GapBuffer::getLineFrom(size_t startIndex, size_t endIndex, size_t size) const{
    std::string result;
    result.reserve(size);

    size_t logicalEnd = startIndex + size;
    logicalEnd = MIN(logicalEnd, endIndex);

    size_t gapSize = gapEnd - gapStart;

    size_t leftStart = startIndex;
    size_t leftEnd   = MIN(logicalEnd, gapStart);

    if (leftStart < leftEnd) result.append(buffer.begin() + leftStart, buffer.begin() + leftEnd);

    size_t rightStart = MAX(startIndex, gapStart);
    size_t rightEnd   = logicalEnd;

    if (rightStart < rightEnd) result.append(buffer.begin() + rightStart + gapSize, buffer.begin() + rightEnd + gapSize);

    return result;
}