#define TEST 1
#include "main.h"

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
   __setup();

}

// TODO состояния:
// - диапазоны датчика 
// - проверки вхожения значения в диапазон (-1/0/1) )
// - настройки состояний реле
// - ссылки на соседние состояния
// TODO переменная указывающая на текущее состояние
// TODO разрешать переключение не раньше, чем N циклов предыдущего переключения (для стабилизации RMS)

void loop() {
//  Serial.print(".");
  uint16_t A0 = analogRead(A0);
  __loop(A0);
}
