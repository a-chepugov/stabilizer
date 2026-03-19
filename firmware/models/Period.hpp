#pragma once
#include <stdint.h>

class Period {
public:
    uint16_t duration;
    uint16_t rms;

    constexpr Period() : duration(0), rms(0) {}
    constexpr Period(uint16_t duration, uint16_t rms) : duration(duration), rms(rms) {}
};
