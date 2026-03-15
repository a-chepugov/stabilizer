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

void reset1() {
    Serial.println("state 1");
}

void reset2() {
    Serial.println("state 2");
}

State<int> state1(1, 2, reset1);
State<int> state2(3, 4, reset2);

inline void link() {
    state2.below = &state1;
};

#endif