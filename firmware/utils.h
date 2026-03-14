#ifndef UTILS_H
#define UTILS_H

namespace utils {
    // Линейная интерполяция положения точки как процент от длины AB (0..1)
    constexpr float spacing(float a, float b, float passed) {
        return a * (1 - passed) + b * passed;
    }

    uint16_t rms(const uint16_t* data, size_t length) {
        size_t count = 0;
        uint32_t sum = 0;
        for (size_t i = 0; i < length; i++) {
            if (data[i] > 0) {
                count++;
                sum += (uint32_t)data[i] * (uint32_t)data[i];           
            }
        }
        return (uint16_t) (sqrt(sum / count));
    }
}

#endif
