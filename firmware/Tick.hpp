#pragma once
#include <stdint.h>

class Tick {
public:
    uint16_t ticks = 0;

    uint16_t old = 0;
    uint16_t mid = 0;

    bool tick(uint16_t now) {
        if (
            // вариант для непрерывного замера, но не работает при дискретных замерах
            // (old < mid && mid > now)

            (old <= mid && mid > now) ||
            (old < mid && mid >= now)
        ) {
            old = mid;
            mid = now;
            ticks++;
            return true;
        } else {
            old = mid;
            mid = now;
            return false;
        }
    }
};
