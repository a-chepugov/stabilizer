#define TEST 1

#include "data.h"
#include "cycle.h"
#include "Transformer.h"

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

  Serial.println("\n====");
   Serial.print("divider_factor: ");
   Serial.println(divider_factor);
   Serial.print("diode_drop_cV: ");
   Serial.println(diode_drop_cV);
   Serial.print("factor_k2_to_k5: ");
   Serial.println(factor_k2_to_k5);

   Serial.println("***");
   
   for(size_t i = 0; i < 4; i++) {
      Serial.print(ranges_adc[i].lo);
      Serial.print(' ');
      Serial.print(ranges_adc[i].hi);
      Serial.println(' ');
   }

   Serial.println("***");


   Serial.println("++++");    
}

char buf[80];

uint16_t it = 0;


// TODO вставить учет падения напряжения на диоде при кассчете коэффициента преобразования
// TODO определить точку смены цикла
// TODO разбить данные на циклы, и аггрегировать данные по ним
// TODO разрешать переключение не раньше, чем N циклов предыдущего переключения (для стабилизации RMS)


void loop() {
  sprintf(buf, "%3d|", it);
  Serial.print(buf);

  digitalWrite(Rel5, HIGH);
  digitalWrite(Rel8, HIGH);
  
  uint16_t A0 = analogRead(A0);
  if (A0 > 0) {
    data_push(A0); 
    if (set_and_tick(A0)) { it++; }
  }

  uint16_t rms = data_rms();

  sprintf(buf, "A0=%4d, rms=%4d | mV: A0=%3d, rms=%4d  | V: A0=%3d, rms=%4d", A0, rms, adc_to_direct(A0), adc_to_direct(rms), adc_to_cV(A0), adc_to_cV(rms) / 100);
  Serial.println(buf);
}
