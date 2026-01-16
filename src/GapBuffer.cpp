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
