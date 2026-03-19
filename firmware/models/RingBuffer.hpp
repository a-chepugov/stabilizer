#pragma once
#include <stddef.h>

template<typename T, size_t SizeLog2>
class RingBuffer {
public:
    static constexpr size_t capacity = 1UL << SizeLog2;
    static constexpr size_t mask = capacity - 1;

    T data[capacity]{};
    size_t head = 0;

    constexpr RingBuffer() = default;

    constexpr T& operator[](size_t i) { return data[i]; }
    const constexpr T& operator[](size_t i) const { return data[i]; }

    void push(const T& value) {
        data[head] = value;
        head = (head + 1) & mask;
    }

    void flush() {
        for (size_t i = 0; i < capacity; ++i) {
            data[i] = T{};
        }
        head = 0;
    }

    void rewind() {
        head = 0;
    }
};
