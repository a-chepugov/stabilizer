#pragma once

#include "RingBuffer.hpp"
#include "Tick.hpp"
#include "Period.hpp"

constexpr uint8_t RING_SIZE_LOG2 = 8;
constexpr uint16_t PROBE_DATA_SIZE = 1UL << RING_SIZE_LOG2;

constexpr uint8_t RECORDS_SIZE_LOG2 = 4;
constexpr uint16_t RECORDS_DATA_SIZE = 1UL << RECORDS_SIZE_LOG2;

constexpr float FREQUENCY = 50;
constexpr float CYCLE_TIME = 1000000 / FREQUENCY;
constexpr uint16_t DELAY_MCS = CYCLE_TIME / PROBE_DATA_SIZE;

class Periods : public Tick {
private:
    uint16_t rms(const uint32_t* data, size_t length) {
        uint32_t sum = 0;
        for (size_t i = 0; i < length; i++) {
            sum += data[i];
        }
        return (uint16_t)(sqrt(sum / length));
    }

public:
    RingBuffer<uint32_t, RING_SIZE_LOG2> raw;
    RingBuffer<Period, RECORDS_SIZE_LOG2> records;

    uint32_t timestamp = 0;

    constexpr Periods(): timestamp(0) {}

    bool push(uint16_t value) {
        bool t = tick(value);
        uint16_t flip = ticks & 0b11;

        //////
        uint32_t data = (((uint32_t)value * value));
        /////

        if (t && flip == 0) {
            uint32_t now = millis();
            uint16_t duration = now - timestamp;
            timestamp = now;

            uint16_t rms = Periods::rms(raw.data, raw.head);

            records.push(Period(duration, rms));



            raw.rewind();
            raw.push(data);
            // raw.push(value);
            return true;
        } else {
            raw.push(data);
            // raw.push(value);
            return false;
        }
    }
};
