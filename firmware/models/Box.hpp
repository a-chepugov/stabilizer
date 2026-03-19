#pragma once

template<typename T>
class Box {
public:
    T value;

    constexpr Box(): value() {}
    constexpr Box(T v): value(v) {}

    template<typename T2, typename Fn>
    constexpr Box<T2> map(Fn fn) const { return Box<T2>(fn(value)); }

    template<typename T2, typename Fn>
    constexpr Box<T2> operator()(Fn fn) const { return Box<T2>(fn(value)); }
};
