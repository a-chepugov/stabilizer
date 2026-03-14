#define TEST 1
#include "Periods.h"

constexpr uint8_t Rel5 = 5;
constexpr uint8_t Rel6 = 6;
constexpr uint8_t Rel7 = 7;
constexpr uint8_t Rel8 = 8;

void setup() {
  pinMode(Rel5, OUTPUT); 
  pinMode(Rel6, OUTPUT); 
  pinMode(Rel7, OUTPUT); 
  pinMode(Rel8, OUTPUT); 

  digitalWrite(Rel5, LOW);
  digitalWrite(Rel6, LOW);
  digitalWrite(Rel7, LOW);
  digitalWrite(Rel8, LOW);

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

//   Serial.print("probe_to_in_factor_full: ");
//   Serial.println(probe_to_in_factor_full);

   Serial.println("***");
   
//   for(size_t i = 0; i < 4; i++) {
//      Serial.print(ranges_adc[i].lo);
//      Serial.print(' ');
//      Serial.print(ranges_adc[i].hi);
//      Serial.println(' ');
//   }

   Serial.println("***");

   Serial.println("+++");    
}

// TODO вставить учет падения напряжения на диоде при кассчете коэффициента преобразования
// TODO определить точку смены цикла
// TODO разбить данные на циклы, и аггрегировать данные по ним
// TODO разрешать переключение не раньше, чем N циклов предыдущего переключения (для стабилизации RMS)


void loop() {
  uint16_t A0 = analogRead(A0);
  action(A0);
}
