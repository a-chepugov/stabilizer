#pragma once
#include <stdint.h>

class Period {
public:
    const uint16_t duration;
    const uint16_t rms;

    constexpr Period() : duration(0), rms(0) {}
    constexpr Period(uint16_t duration, uint16_t rms) : duration(duration), rms(rms) {}

    constexpr Period operator=(const Period& other) const { return other; }
};
