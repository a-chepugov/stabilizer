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

  Serial.begin(9600);

  Serial.println("++++");

  for(uint16_t i = 0; i < 4; i++) {
    for(uint16_t j = 0; j < 2; j++) {
       Serial.print(' ');
        Serial.print(spacing_kt[i][j]);
    }
    Serial.println("");
  }
    Serial.println("***");

  for(uint16_t i = 0; i < 4; i++) {
    for(uint16_t j = 0; j < 2; j++) {
       Serial.print(' ');
        Serial.print(spacing_cV[i][j]);
    }
    Serial.println("");
  }
    Serial.println("***");

  for(uint16_t i = 0; i < 4; i++) {
    for(uint16_t j = 0; j < 2; j++) {
       Serial.print(' ');
        Serial.print(spacing_adc[i][j]);
    }
    Serial.println("");
  }
    Serial.println("***");

  Serial.println("====");
}

char buf[80];

uint16_t it = 0;


// TODO разрешать переключение не раньше, чем N циклов предыдущего переключения (для стабилизации RMS)
// TODO разбить данные на циклы, и аггрегировать данные по ним
// TODO определить 

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
 
//  sprintf(buf, "raw: A0 = %4d, rms = %4d", A0, rms);
//  Serial.println(buf);
//  sprintf(buf, "mV: A0 = %4d, rms = %4d", adc_to_mV(A0), adc_to_mV(rms));
//  Serial.println(buf);
//  sprintf(buf, "V: A0 = %3d, max = %3d, rms = %3d", adc_to_cV(A0) / 100, adc_to_cV(max) / 100, adc_to_cV(rms) / 100);
  sprintf(buf, "A0=%4d, rms=%4d | V: A0=%3d, rms = %3d", A0, rms, adc_to_cV(A0) / 100, adc_to_cV(rms) / 100);
  Serial.println(buf);
//  Serial.println("");
}
