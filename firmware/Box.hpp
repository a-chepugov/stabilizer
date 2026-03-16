#pragma once

template<typename T>
struct Box {
    T value;

    constexpr Box(T value): value(value) {}

    constexpr get() { return value; }

    template<typename T2, typename Fn>
    constexpr Box<T2> operator()(Fn fn) const { return Box<T2>(fn(value)); }

    template<typename T2, typename Fn>
    constexpr Box<T2> map(Fn fn) const {
      return Box<T2>(fn(value));
    }
};
