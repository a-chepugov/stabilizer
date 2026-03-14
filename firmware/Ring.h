#ifndef RING_H
#define RING_H

template<typename T>
class RingBuffer {
private:
public:
    T* data;
    size_t head = 0;
    size_t capacity;
    size_t mask;

    // size_log2 — логарифм размера буфера по основанию 2
    // например: 8 → 256 элементов, 10 → 1024 элементов
    explicit RingBuffer(size_t size_log2) {
        capacity = 1UL << size_log2;
        mask = capacity - 1;

        data = new T[capacity];
    }

    ~RingBuffer() {
        delete[] data;
    }

    void push(const T& value) {
        data[head] = value;
        head = (head + 1) & mask;
    }

    void flush() {
        memset(data, 0, sizeof(T) * capacity);
        head = 0;
    }
};

#endif
