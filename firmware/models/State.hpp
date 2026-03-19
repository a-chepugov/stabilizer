#pragma once

template<typename T>
class State {
public:
    const T lo;
    const T hi;

    State<T> * below = nullptr;
    State<T> * above = nullptr;

    typedef void (*func)();

    func apply;
    
    constexpr State(
        T lo, T hi, func apply
    ): lo(lo), hi(hi), apply(apply) {}

    int8_t check(T value) const {      
        if (value < lo) return -1;
        if (value > hi) return 1;
        return 0;
    }
};
