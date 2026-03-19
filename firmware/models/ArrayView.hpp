#pragma once
#include <stddef.h>

template<typename T, size_t N>
struct ArrayView
{
    T data[N];
    static constexpr size_t length = N;

    constexpr ArrayView() : data{} {}

    // Копирование из C-массива
    // constexpr ArrayView(const T (&arr)[N]) : data{} {
    //     for (size_t i = 0; i < N; ++i) data[i] = arr[i];
    // }
    constexpr ArrayView(const T (&arr)[N]) : data(arr) {}

    constexpr T& operator[](size_t i) { return data[i]; }
    const constexpr T& operator[](size_t i) const { return data[i]; }

    template<typename T2, typename F>
    constexpr ArrayView<T2, N>map(F func) {
        ArrayView<T2, N> result{};

        for (size_t i = 0; i < this->length; ++i) {
            result[i] = func(data[i]);
        }

        return result;
    }
};
