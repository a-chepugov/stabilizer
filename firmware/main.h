#include "Transformer.h"

#include "Periods.h"
#include "State.h"

Periods periods;

inline void __setup() {
    link();
    state2.apply();
    Serial.print("DELAY_MCS:");
    Serial.println(DELAY_MCS);

    // Serial.println(square_table[10]); // 100
    // Serial.println(square_table[1023]); // 1046529
}

inline void __loop(uint16_t A0) {
    periods.push(A0);
    delayMicroseconds(DELAY_MCS);
}

