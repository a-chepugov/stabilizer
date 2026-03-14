#ifndef PERIODS_H
#define PERIODS_H

#include "Ring.h"
#include "Tick.h"
#include "Period.h"
#include "utils.h"
#include "Transformer.h"

char buffer[80];

class Periods : public Tick {
public:
    RingBuffer<uint16_t> raw = RingBuffer<uint16_t>(7);
    RingBuffer<Period> periods = RingBuffer<Period>(5);
    uint32_t timestamp = 0;

    Periods() {
        raw.flush();
    }

    void push(uint16_t value) {
        if (value > 0) {
            if (tick(value)) {
                uint32_t now = millis();
                uint32_t duration = now - timestamp;
                timestamp = now;

                uint16_t r = rms(raw.data, raw.head);

                periods.push(Period(duration, r));

                sprintf(buffer, "%3d > pr=%3d : raw=%4d | p=%4d | V=%4d", ticks, raw.head, r, adc_to_direct(r), adc_to_cV(r) / 100);
                Serial.println(buffer);

                raw.flush();
            };

            raw.push(value);
        }
    }
        
};

Periods periods;

void action(uint16_t A0) {
    periods.push(A0);
}

#endif