#include <stdint.h>
#include <Arduino.h>
#include <HardwareSerial.h>

#include "cfg/cfg.hpp"
#include "cfg/Transformer.hpp"

#include "models/Periods.hpp"
#include "models/State.hpp"

constexpr uint8_t Relay5 = 5;
constexpr uint8_t Relay6 = 6;
constexpr uint8_t Relay7 = 7;
constexpr uint8_t Relay8 = 8;

int status;

void connect_to_4() {
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, HIGH);
  digitalWrite(Relay7, HIGH);
  digitalWrite(Relay8, HIGH);
  Serial.println("state 4");
  status = 4;
}

void connect_to_5() {
  digitalWrite(Relay5, LOW);
  digitalWrite(Relay6, LOW);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, HIGH);
  Serial.println("state 5");
  status = 5;
}

void connect_to_6() {
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, LOW);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, HIGH);
  Serial.println("state 6");
  status = 6;
}

void connect_to_7() {
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, HIGH);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, HIGH);
  Serial.println("state 7");
  status = 7;
}

void disconnect() {
  digitalWrite(Relay8, LOW);
  Serial.println("disconnect");
  status = 0;
}

State<uint16_t> state_crit_hi{ cfg::adc_ranges[0], 0xFFFF, disconnect };

State<uint16_t> state_4{ cfg::adc_ranges[2], cfg::adc_ranges[1], connect_to_4 };
State<uint16_t> state_5{ cfg::adc_ranges[4], cfg::adc_ranges[3], connect_to_5 };
State<uint16_t> state_6{ cfg::adc_ranges[6], cfg::adc_ranges[5], connect_to_6 };
State<uint16_t> state_7{ cfg::adc_ranges[8], cfg::adc_ranges[7], connect_to_7 };

State<uint16_t> state_crit_lo{ 0, cfg::adc_ranges[9], disconnect };

State<uint16_t> * current_state = &state_crit_hi;

inline void link() {
    state_crit_hi.below = &state_4;

    state_4.above = &state_crit_hi;
    state_4.below = &state_5;

    state_5.above = &state_4;
    state_5.below = &state_6;

    state_6.above = &state_5;
    state_6.below = &state_7;

    state_7.above = &state_6;
    state_7.below = &state_crit_lo;

    state_crit_lo.above = &state_7;
};

void setup() {
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);

  link();
  current_state->apply();

  Serial.begin(9600);
  Serial.println("\n===");
  Serial.println("123");


  //  Serial.print("k_crit_min: ");
  //  Serial.println(k_crit_min);
  //  Serial.print("k_crit_max: ");
  //  Serial.println(k_crit_max);

  // for (size_t i = 0; i < cfg::t_k_tuples.length; ++i) {
  //   Serial.print(cfg::t_k_tuples[i].a);
  //   Serial.print(" ");
  //   Serial.print(cfg::t_k_tuples[i].b);
  //   Serial.print(" ");
  //   Serial.print(cfg::t_k_tuples[i].part);
  //   Serial.println("");
  // }

  // for (size_t i = 0; i < cfg::t_k_ranges.length; ++i) {
  //   Serial.println(cfg::t_k_ranges[i]);
  // }

  // for (size_t i = 0; i < cfg::cV_ranges.length; ++i) {
  //   Serial.println(cfg::cV_ranges[i]);
  // }

  // for (size_t i = 0; i < cfg::adc_ranges.length; ++i) {
  //   Serial.println((float)cfg::adc_ranges[i]);
  // }

  // Serial.println("|-|");
  // Serial.println(adc_to_cV(298)); // 150.00
  // Serial.println(adc_to_cV(470)); // 230.00
  // Serial.println(adc_to_cV(650)); // 315.00
  // Serial.println(adc_to_cV(1012)); // 485.00
  // Serial.println("||");
  // Serial.println(cV_to_adc(15000)); // 298
  // Serial.println(cV_to_adc(23000)); // 469
  // Serial.println(cV_to_adc(31500)); // 650
  // Serial.println(cV_to_adc(48500)); // 1012
  // Serial.println("|-|");

  //  Serial.print("i_test_to_in_factor: ");
  //  Serial.println(i_test_to_in_factor);

  //  Serial.print("diode_drop_cV: ");
  //  Serial.println(diode_drop_cV);

  //  Serial.print("divider_factor: ");
  //  Serial.println(divider_factor);

  //  Serial.print("probe_to_divider_factor: ");
  //  Serial.println(probe_to_divider_factor);

  // Serial.print("DELAY_MCS:");
  // Serial.println(DELAY_MCS);




//   Serial.println("***");

//   for(size_t i = 0; i < 4; i++) {
//      Serial.print(ranges_adc[i].lo);
//      Serial.print(' ');
//      Serial.print(ranges_adc[i].hi);
//      Serial.println(' ');
//   }

   Serial.println("+++");
}

Periods periods{};

char buffer[80];

uint16_t avg() {
  uint32_t sum = 0;
  size_t count = 0;
  for (size_t i = 0; i < RECORDS_DATA_SIZE; ++i) {
    if (periods.records.data[i].rms > 0) {
      sum += periods.records.data[i].rms;
      ++count;
    }
  }

  return sum / count;
}


void loop() {
  // Serial.print(".");
  uint16_t A0 = analogRead(A0);
  if (A0 > 0) {
    size_t head = periods.adc.head;
    if (periods.push(A0)) {
        uint16_t rms = avg();
        int8_t status_check = current_state->check(rms);
        if (status_check == 1) {
          current_state = current_state->above;
          current_state->apply();
        } else if (status_check == -1) {
          current_state = current_state->below;
          current_state->apply();
        }

        // sprintf(buffer, "t%3d : h %4d > pin=%4d | d=%3d | V=%3d", periods.ticks, head, rms, adc_to_direct(rms), adc_to_cV(rms));
        sprintf(buffer, "t:%3d |h:%3d > r:%4d | V:%3d (s:%1d|%2d)", periods.ticks, head, rms, tr::adc_to_cV(rms), status, status_check);
        Serial.println(buffer);
    }
  }
  delayMicroseconds(DELAY_MCS);
}
