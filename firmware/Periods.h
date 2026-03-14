#ifndef PERIODS_H
#define PERIODS_H

#include "Ring.h"
#include "Tick.h"
#include "Period.h"
#include "utils.h"
#include "Transformer.h"

char buffer[80];

uint8_t td = 0;

class Periods : public Tick {
public:
    RingBuffer<uint16_t, 9> raw;
    RingBuffer<Period, 5> periods;

    uint32_t timestamp = 0;

    constexpr Periods() {}

    void push(uint16_t value) {
        if (value > 0) {
            bool t = tick(value);
            if  (t) {
                td = (td + 1) % 2;
            }

            if (t && td != 1) {
                uint32_t now = millis();
                uint32_t duration = now - timestamp;
                timestamp = now;

                uint16_t rms = utils::rms(raw.data, raw.head);

                periods.push(Period(duration, rms));

                sprintf(buffer, "%3d : pr=%3d >> raw=%4d | d=%3d | V=%3d", ticks, raw.head, rms, adc_to_direct(rms), adc_to_cV(rms) / 100);
                Serial.println(buffer);

                raw.rewind();
            };
            raw.push(value);
        }
    }
        
};

Periods periods;

void action(uint16_t A0) {
    periods.push(A0);
    delayMicroseconds(50);
}

#endif