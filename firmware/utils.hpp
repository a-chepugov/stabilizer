#pragma once
#include <stdint.h>
#include <stddef.h>

namespace utils {
    // Линейная интерполяция положения точки как процент от длины AB (0..1)
    constexpr float spacing(float a, float b, float part) {
        return a * (1 - part) + b * part;
    }

    constexpr uint16_t rms(const uint16_t* data, size_t length) {
        uint32_t sum = 0;
        for (size_t i = 0; i < length; i++) {
            sum += (uint32_t)data[i] * (uint32_t)data[i];
        }
        return (uint16_t) (sqrt(sum / length));
    }

}
