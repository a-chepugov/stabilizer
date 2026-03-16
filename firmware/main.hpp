#include "Arduino.h"
#include "Transformer.hpp"

#include "Periods.hpp"
#include "State.hpp"

constexpr uint8_t Relay5 = 5;
constexpr uint8_t Relay6 = 6;
constexpr uint8_t Relay7 = 7;
constexpr uint8_t Relay8 = 8;

void connect_to_5() {
  digitalWrite(Relay5, LOW);
  digitalWrite(Relay6, LOW);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, HIGH);
}

void connect_to_6() {
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, LOW);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, HIGH);
}

void connect_to_7() {
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, HIGH);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, HIGH);
}

void connect_to_4() {
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, HIGH);
  digitalWrite(Relay7, HIGH);
  digitalWrite(Relay8, HIGH);
}

void disconnect() {
  digitalWrite(Relay8, LOW);
}

State<int> state_crit_lo(3, 4, disconnect);

State<int> state_4(1, 2, connect_to_4);
State<int> state_5(3, 4, connect_to_5);
State<int> state_6(3, 4, connect_to_6);
State<int> state_7(3, 4, connect_to_7);

State<int> state_crit_hi(3, 4, disconnect);

inline void link() {
    state_4.below = &state_crit_lo;
    state_4.above = &state_5;

    state_5.below = &state_4;
    state_5.above = &state_6;
    
    state_6.below = &state_5;
    state_6.above = &state_7;
    
    state_7.below = &state_6;
    state_7.above = &state_crit_hi;
};

void setup() {
  pinMode(Relay5, OUTPUT); 
  pinMode(Relay6, OUTPUT); 
  pinMode(Relay7, OUTPUT); 
  pinMode(Relay8, OUTPUT); 

  disconnect();

  Serial.begin(9600);
  Serial.println("\n===");

//   Serial.print("test_to_in_factor: ");
//   Serial.println(test_to_in_factor);

//   Serial.print("diode_drop_cV: ");
//   Serial.println(diode_drop_cV);

//   Serial.print("divider_factor: ");
//   Serial.println(divider_factor);

//   Serial.print("probe_to_in_factor: ");
//   Serial.println(probe_to_in_factor);

//   Serial.println("***");
   
//   for(size_t i = 0; i < 4; i++) {
//      Serial.print(ranges_adc[i].lo);
//      Serial.print(' ');
//      Serial.print(ranges_adc[i].hi);
//      Serial.println(' ');
//   }

   Serial.println("***");
   Serial.println("+++");

   link();
   Serial.print("DELAY_MCS:");
   Serial.println(DELAY_MCS);
}

Periods periods;

void loop() {
//  Serial.print(".");
  uint16_t A0 = analogRead(A0);
  periods.push(A0);
  delayMicroseconds(DELAY_MCS);
}
