#pragma once

#include "Ring.hpp"
#include "Tick.hpp"
#include "Period.hpp"
#include "utils.hpp"

constexpr uint8_t RING_SIZE_LOG2 = 8;
constexpr uint16_t RING_SIZE = 1UL << RING_SIZE_LOG2;

constexpr float FREQUENCY = 50;
constexpr float CYCLE_TIME = 1000000 / FREQUENCY;
constexpr uint16_t DELAY_MCS = CYCLE_TIME / RING_SIZE;

char buffer[80];

class Periods : public Tick {
public:
    RingBuffer<uint16_t, RING_SIZE_LOG2> raw;
    RingBuffer<Period, 5> periods;

    uint32_t timestamp = 0;

    constexpr Periods() {}

    void push(uint16_t value) {
        if (value > 0) {
            bool t = tick(value);
            uint16_t flip = ticks & 0b11;

            if (t && flip == 0) {
                uint32_t now = millis();
                uint16_t duration = now - timestamp;
                timestamp = now;

                uint16_t rms = utils::rms(raw.data, raw.head);

                periods.push(Period(duration, rms));

                sprintf(buffer, "\n%3d : pr=%3d >> raw=%4d | d=%3d | V=%3d", ticks, raw.head, rms, adc_to_direct(rms), adc_to_cV(rms));
                Serial.println(buffer);

                raw.rewind();
            };
            raw.push(value);
        }
    }
};
