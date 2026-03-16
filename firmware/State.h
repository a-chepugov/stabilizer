#ifndef STATE_H
#define STATE_H

template<typename T>
class State {
public:
    const T lo;
    const T hi;

    State<T> * below = nullptr;
    State<T> * above = nullptr;

    typedef void (*func)();

    func apply = nullptr;
    
    constexpr State(
        T lo, T hi, func apply = nullptr
    ): lo(lo), hi(hi), apply(apply) {}

    inline uint8_t check(T value) const {
        if (value < lo) return -1;
        if (value > hi) return 1;
        return 0;
    }
};

#endif