#define TEST 1

#include "data.h"
#include "cycle.h"

// Коеэффициенты преобразования трансформатора
constexpr uint16_t k7 = 100;
constexpr uint16_t k6 = 90;
constexpr uint16_t k5 = 80;
constexpr uint16_t k4 = 65;
constexpr uint16_t k3 = 11;
constexpr uint16_t k2 = 10;
constexpr uint16_t k1 = 5;

constexpr uint16_t target_k = k5; // входной контакт
constexpr uint16_t target_cV = 23000; // 230.00

constexpr double efficiency = 0.98;

// резисторы на делителе напряжения
constexpr double R1 = 100000;
constexpr double R2 = 10000;

constexpr double divider_factor = R2 / (R2 + R1);

constexpr uint16_t VREF_mV = 5000; // Милливольты
constexpr uint16_t VREF_cV = VREF_mV / 10; // Сантивольты

constexpr uint16_t ADC_MAX_BIT = 10;

constexpr uint16_t adc_to_mV(uint16_t adc) { // Показания на датчике
  return (uint16_t)(((uint32_t)adc * VREF_mV) >> ADC_MAX_BIT);
}

constexpr double factor_k2_to_k5 = (double)k5 / k2 / divider_factor / efficiency;
constexpr uint16_t actual_factor_k2_to_k5 = (uint16_t)(VREF_cV * factor_k2_to_k5);

constexpr uint16_t adc_to_cV(uint16_t adc) {
  return (uint16_t)(((uint32_t)adc * actual_factor_k2_to_k5) >> ADC_MAX_BIT);
}

constexpr uint16_t cV_to_adc(uint16_t cV) {
  return (uint16_t)(((uint32_t)cV << ADC_MAX_BIT) / actual_factor_k2_to_k5);
}

// вычисляем положение точки как процент от длины AB
constexpr double spacing(double a, double b, double passed) {
    return a * (1 - passed) + b * passed;
}

constexpr double Vtrans(uint16_t k) {
    return (double)target_cV * k / target_k;
}

constexpr double convert(uint16_t a_cV, uint16_t b_cV, uint16_t passed) {
    return (a_cV * (100 - passed) + b_cV * passed) / 100;
}

uint16_t spacing_kt[4][2] = {
  {                     0, spacing(k4, k5, 0.57) },
  { spacing(k4, k5, 0.43), spacing(k5, k6, 0.57) },
  { spacing(k5, k6, 0.43), spacing(k6, k7, 0.57) },
  { spacing(k6, k7, 0.43), 0xFFFF },
};

uint16_t spacing_cV[4][2] = {
  {                        0, Vtrans(spacing_kt[0][1]) },
  { Vtrans(spacing_kt[1][0]), Vtrans(spacing_kt[1][1]) },
  { Vtrans(spacing_kt[2][0]), Vtrans(spacing_kt[2][1]) },
  { Vtrans(spacing_kt[3][0]), 0xFFFF },
};

uint16_t spacing_adc[4][2] = {
  {                           0, cV_to_adc(spacing_cV[0][1]) },
  { cV_to_adc(spacing_cV[1][0]), cV_to_adc(spacing_cV[1][1]) },
  { cV_to_adc(spacing_cV[2][0]), cV_to_adc(spacing_cV[2][1]) },
  { cV_to_adc(spacing_cV[3][0]), 0xFFFF },
};

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
