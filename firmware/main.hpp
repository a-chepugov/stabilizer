#include <stdint.h>
#include <Arduino.h>
#include <HardwareSerial.h>

#include "cfg/cfg.hpp"
#include "hw/hw.hpp"

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

State<uint16_t> state_hi{ cfg::adc_ranges[0], 0xFFFF, disconnect };

State<uint16_t> state_4{ cfg::adc_ranges[2], cfg::adc_ranges[1], connect_to_4 };
State<uint16_t> state_5{ cfg::adc_ranges[4], cfg::adc_ranges[3], connect_to_5 };
State<uint16_t> state_6{ cfg::adc_ranges[6], cfg::adc_ranges[5], connect_to_6 };
State<uint16_t> state_7{ cfg::adc_ranges[8], cfg::adc_ranges[7], connect_to_7 };

State<uint16_t> state_lo{ 0, cfg::adc_ranges[9], disconnect };

State<uint16_t> * current_state = &state_hi;

inline void link() {
    state_hi.below = &state_4;

    state_4.above = &state_hi;
    state_4.below = &state_5;

    state_5.above = &state_4;
    state_5.below = &state_6;

    state_6.above = &state_5;
    state_6.below = &state_7;

    state_7.above = &state_6;
    state_7.below = &state_lo;

    state_lo.above = &state_7;
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
  Serial.println(__TIME__);

  // Serial.print("OUTPUT_cV_RMS_TARGET: ");
  // Serial.println(hw::OUTPUT_cV_RMS_TARGET);
  // Serial.print("OUTPUT_cV_RMS_MIN: ");
  // Serial.println(hw::OUTPUT_cV_RMS_MIN);
  // Serial.print("OUTPUT_cV_RMS_MAX: ");
  // Serial.println(hw::OUTPUT_cV_RMS_MAX);

  // Serial.print("INPUT_cV_RMS_MIN: ");
  // Serial.println(hw::INPUT_cV_RMS_MIN);
  // Serial.print("INPUT_cV_RMS_MAX: ");
  // Serial.println(hw::INPUT_cV_RMS_MAX);

  // Serial.print("k_crit_hi: ");
  // Serial.println(hw::k_crit_hi);
  // Serial.print("k_crit_lo: ");
  // Serial.println(hw::k_crit_lo);

  //  Serial.print("test_to_in_factor: ");
  //  Serial.println(hw::test_to_in_factor);

  //  Serial.print("diode_drop: ");
  //  Serial.println(hw::diode_drop);

  //  Serial.print("divider_factor: ");
  //  Serial.println(hw::divider_factor);

  // Serial.print("DELAY_MCS:");
  // Serial.println(DELAY_MCS);

  Serial.println("***");

  Serial.println(hw::adc_to_V(517)); //  (V) 221.06
  Serial.println(hw::adc_to_cV(517)); // (V) 22096
  Serial.println(hw::V_to_adc(220.0f)); // (adc) 517
  Serial.println(hw::cV_to_adc(22000)); // (adc) 517

  Serial.println("|---|");
  Serial.println(hw::adc_to_V(127)); //  (V)  61.24
  Serial.println(hw::adc_to_V(255)); //  (V) 113.30
  Serial.println(hw::adc_to_V(512)); //  (V) 217.80
  Serial.println(hw::adc_to_V(1023)); // (V) 425.60
  Serial.println("|*|");
  Serial.println(hw::adc_to_cV(127)); //  (V)  6112
  Serial.println(hw::adc_to_cV(255)); //  (V) 11312
  Serial.println(hw::adc_to_cV(512)); //  (V) 21776
  Serial.println(hw::adc_to_cV(1023)); // (V) 42544
  Serial.println("|-|");
  Serial.println(hw::V_to_adc(150.0f)); // (adc) 345
  Serial.println(hw::V_to_adc(230.0f)); // (adc) 541
  Serial.println(hw::V_to_adc(315.0f)); // (adc) 751
  Serial.println(hw::V_to_adc(425.0f)); // (adc) 1021
  Serial.println("|*|");
  Serial.println(hw::cV_to_adc(15000)); // (adc) 345
  Serial.println(hw::cV_to_adc(23000)); // (adc) 541
  Serial.println(hw::cV_to_adc(31500)); // (adc) 750
  Serial.println(hw::cV_to_adc(42500)); // (adc) 1021
  Serial.println("|---|");


  // Serial.println("t_k_tuples");
  // for (size_t i = 0; i < cfg::t_k_tuples.length; ++i) {
  //   Serial.print(cfg::t_k_tuples[i].a);
  //   Serial.print(" ");
  //   Serial.print(cfg::t_k_tuples[i].b);
  //   Serial.print(" ");
  //   Serial.print(cfg::t_k_tuples[i].part);
  //   Serial.println("");
  // }

  // Serial.println("t_k_ranges");
  // for (size_t i = 0; i < cfg::t_k_ranges.length; ++i) {
  //   Serial.println(cfg::t_k_ranges[i]);
  // }

  // Serial.println("cV_ranges");
  // for (size_t i = 0; i < cfg::cV_ranges.length; ++i) {
  //   Serial.println(cfg::cV_ranges[i]);
  // }

  // Serial.println("adc_ranges");
  // for (size_t i = 0; i < cfg::adc_ranges.length; ++i) {
  //   Serial.println((float)cfg::adc_ranges[i]);
  // }
  
  Serial.println("states");
  State<uint16_t> states[6] = { state_hi, state_4, state_5, state_6, state_7, state_lo };
  for(size_t i = 0; i < 6; i++) {
     Serial.print(states[i].lo);
     Serial.print(' ');
     Serial.print(states[i].hi);
     Serial.println(' ');
  }

  // 705 65535 

  // 567 708 
  
  // 482 579 
  
  // 429 488 
  
  // 344 434 
  
  // 0 347 
  

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

        // sprintf(buffer, "t%3d : h %4d > pin=%4d | d=%3d | V=%3d", periods.ticks, head, rms, adc:to_cV(rms), adc_to_cV(rms));
        sprintf(buffer, "t:%3d |h:%3d > r:%4d | V:%3d (s:%1d|%2d)", periods.ticks, head, rms, hw::adc_to_cV(rms), status, status_check);
        Serial.println(buffer);
    }
  }
  delayMicroseconds(DELAY_MCS);
}
