#pragma once

template<typename T, size_t N>
struct ConstArray
{
    T data[N];

    constexpr ConstArray() : data{} {}

    // Копирование из C-массива
    constexpr ConstArray(const T (&arr)[N]) : data{} {
        for (size_t i = 0; i < N; ++i) data[i] = arr[i];
    }

    static constexpr size_t length = N;

    constexpr T& operator[](size_t i) { return data[i]; }
    const constexpr T& operator[](size_t i) const { return data[i]; }

    template<typename T2, typename F>
    constexpr ConstArray<T2, N>map(F func) {
        ConstArray<T2, N> result{};
        
        for (size_t i = 0; i < this->length; ++i) {          
            result[i] = func(data[i]);
        }
    
        return result;
    }
};