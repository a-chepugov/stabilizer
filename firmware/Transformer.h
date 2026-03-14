#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "Range.h"

// Коэффициенты преобразования трансформатора (отводы)
constexpr uint16_t k7 = 100;
constexpr uint16_t k6 = 90;
constexpr uint16_t k5 = 80;
constexpr uint16_t k4 = 65;
constexpr uint16_t k3 = 11;
constexpr uint16_t k2 = 10;
constexpr uint16_t k1 = 5;

// Целевые параметры
constexpr uint16_t input_k = k5;   // входной контакт
constexpr uint16_t test_k = k2 - k1;   // входной контакт

constexpr uint16_t target_V = 230; // 230.00 В в сантивольтах
constexpr uint16_t target_cV = target_V * 100; // 230.00 В в сантивольтах

constexpr double in_to_test_factor = input_k / test_k;

// КПД трансформатора
constexpr double efficiency = 0.98;

// Номиналы резисторов делителя напряжения
constexpr double R1 = 50000;
constexpr double R2 = 10000;

constexpr double divider_factor = 1 + R1 / R2;

// Опорное напряжение и АЦП
constexpr uint16_t VREF_mV = 5000; // 5.00 В в милливольты
constexpr uint16_t VREF_cV = VREF_mV / 10; // 5.00 В в сантивольты
constexpr uint16_t ADC_MAX_BIT = 10;

constexpr double diode_drop = 0.613127;
constexpr uint16_t diode_drop_cV = diode_drop * 100;

// Пересчёт показаний АЦП в милливольтох на датчике
constexpr uint16_t adc_to_direct(uint16_t adc) {
  return (uint16_t)(((uint32_t)adc * VREF_mV) >> ADC_MAX_BIT);
}

// Коэффициент пересчёта от отвода k2 к k5 с учётом делителя и КПД
constexpr double factor_k2_to_k5 = (double)in_to_test_factor * divider_factor / efficiency;
constexpr uint16_t actual_factor_k2_to_k5 = (uint16_t)(VREF_cV * factor_k2_to_k5);

// Пересчёт АЦП в сантивольты на выходе трансформатора
constexpr uint16_t adc_to_cV(uint16_t adc) {
  return (uint16_t)(((uint32_t)(adc) * actual_factor_k2_to_k5) >> ADC_MAX_BIT);
}

constexpr uint16_t cV_to_adc(uint16_t cV) {
  return (uint16_t)(((uint32_t)cV << ADC_MAX_BIT) / actual_factor_k2_to_k5);
}

// Положение точки как процент от длины AB (0..1)
constexpr double spacing(double a, double b, double passed) {
  return a * (1 - passed) + b * passed;
}

// Напряжение на трансформаторе для отвода k
constexpr double Vtrans(double k) {
  return k * target_cV / input_k;
}

// Линейная интерполяция между a_cV и b_cV по passed (0..100)
constexpr double convert(uint16_t a_cV, uint16_t b_cV, uint16_t passed) {
  return (a_cV * (100 - passed) + b_cV * passed) / 100;
}


// Границы диапазонов по коэффициентам отвода
static constexpr Range<double> ranges_kt[4] = {
  Range<double>(0, spacing(k4, k5, 0.57)),
  Range<double>(spacing(k4, k5, 0.43), spacing(k5, k6, 0.57)),
  Range<double>(spacing(k5, k6, 0.43), spacing(k6, k7, 0.57)),
  Range<double>(spacing(k6, k7, 0.43), 0xFFFF),
};

// Границы диапазонов в сантивольтах (те же выражения, что в spacing_kt)
static constexpr Range<double> ranges_cV[4] = {
  ranges_kt[0].map<double>(Vtrans),
  ranges_kt[1].map<double>(Vtrans),
  ranges_kt[2].map<double>(Vtrans),
  ranges_kt[3].map<double>(Vtrans),
};

// Границы диапазонов в единицах АЦП
static constexpr Range<uint16_t> ranges_adc[4] = {
  ranges_cV[0].map<uint16_t>(cV_to_adc),
  ranges_cV[1].map<uint16_t>(cV_to_adc),
  ranges_cV[2].map<uint16_t>(cV_to_adc),
  ranges_cV[3].map<uint16_t>(cV_to_adc),
};

#endif // TRANSFORMER_H
