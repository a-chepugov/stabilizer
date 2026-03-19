#pragma once
#include <stdint.h>

class Period {
    public:
        uint16_t duration = 0;
        uint16_t rms = 0;

        constexpr Period() = default;

        constexpr Period(const uint16_t duration, const uint16_t rms) : duration(duration), rms(rms) {};
};
