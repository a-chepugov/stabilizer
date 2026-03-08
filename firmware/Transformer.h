#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <stdint.h>

// Коэффициенты преобразования трансформатора (отводы)
constexpr uint16_t k7 = 100;
constexpr uint16_t k6 = 90;
constexpr uint16_t k5 = 80;
constexpr uint16_t k4 = 65;
constexpr uint16_t k3 = 11;
constexpr uint16_t k2 = 10;
constexpr uint16_t k1 = 5;

// Целевые параметры
constexpr uint16_t target_k = k5;   // входной контакт
constexpr uint16_t target_cV = 23000; // 230.00 В в сантивольтах

// КПД трансформатора
constexpr double efficiency = 0.98;

// Резисторы на делителе напряжения
constexpr double R1 = 100000;
constexpr double R2 = 10000;

constexpr double divider_factor = R2 / (R2 + R1);

// Опорное напряжение и АЦП
constexpr uint16_t VREF_mV = 5000;  // милливольты
constexpr uint16_t VREF_cV = VREF_mV / 10;
constexpr uint16_t ADC_MAX_BIT = 10;

// Пересчёт показаний АЦП в милливольты на датчике
constexpr uint16_t adc_to_mV(uint16_t adc) {
  return (uint16_t)(((uint32_t)adc * VREF_mV) >> ADC_MAX_BIT);
}

// Коэффициент пересчёта от отвода k2 к k5 с учётом делителя и КПД
constexpr double factor_k2_to_k5 = (double)k5 / k2 / divider_factor / efficiency;
constexpr uint16_t actual_factor_k2_to_k5 = (uint16_t)(VREF_cV * factor_k2_to_k5);

// Пересчёт АЦП в сантивольты на выходе трансформатора
constexpr uint16_t adc_to_cV(uint16_t adc) {
  return (uint16_t)(((uint32_t)adc * actual_factor_k2_to_k5) >> ADC_MAX_BIT);
}

constexpr uint16_t cV_to_adc(uint16_t cV) {
  return (uint16_t)(((uint32_t)cV << ADC_MAX_BIT) / actual_factor_k2_to_k5);
}

// Положение точки как процент от длины AB (0..1)
constexpr double spacing(double a, double b, double passed) {
  return a * (1 - passed) + b * passed;
}

// Напряжение на трансформаторе для отвода k
constexpr double Vtrans(uint16_t k) {
  return (double)target_cV * k / target_k;
}

// Линейная интерполяция между a_cV и b_cV по passed (0..100)
constexpr double convert(uint16_t a_cV, uint16_t b_cV, uint16_t passed) {
  return (a_cV * (100 - passed) + b_cV * passed) / 100;
}

// Границы диапазонов по коэффициентам отвода
static constexpr uint16_t spacing_kt[4][2] = {
  {                     0, spacing(k4, k5, 0.57) },
  { spacing(k4, k5, 0.43), spacing(k5, k6, 0.57) },
  { spacing(k5, k6, 0.43), spacing(k6, k7, 0.57) },
  { spacing(k6, k7, 0.43), 0xFFFF },
};

// Границы диапазонов в сантивольтах (те же выражения, что в spacing_kt)
static constexpr uint16_t spacing_cV[4][2] = {
  { 0, (uint16_t)Vtrans(spacing(k4, k5, 0.57)) },
  { (uint16_t)Vtrans(spacing(k4, k5, 0.43)), (uint16_t)Vtrans(spacing(k5, k6, 0.57)) },
  { (uint16_t)Vtrans(spacing(k5, k6, 0.43)), (uint16_t)Vtrans(spacing(k6, k7, 0.57)) },
  { (uint16_t)Vtrans(spacing(k6, k7, 0.43)), 0xFFFF },
};

// Границы диапазонов в единицах АЦП
static constexpr uint16_t spacing_adc[4][2] = {
  { 0, cV_to_adc((uint16_t)Vtrans(spacing(k4, k5, 0.57))) },
  { cV_to_adc((uint16_t)Vtrans(spacing(k4, k5, 0.43))), cV_to_adc((uint16_t)Vtrans(spacing(k5, k6, 0.57))) },
  { cV_to_adc((uint16_t)Vtrans(spacing(k5, k6, 0.43))), cV_to_adc((uint16_t)Vtrans(spacing(k6, k7, 0.57))) },
  { cV_to_adc((uint16_t)Vtrans(spacing(k6, k7, 0.43))), 0xFFFF },
};

#endif // TRANSFORMER_H
