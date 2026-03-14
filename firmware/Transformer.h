#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "Range.h"
#include "utils.h"

// Коэффициенты преобразования трансформатора (отводы)
constexpr uint16_t k7 = 100;
constexpr uint16_t k6 = 90;
constexpr uint16_t k5 = 80;
constexpr uint16_t k4 = 65;
constexpr uint16_t k3 = 11;
constexpr uint16_t k2 = 10;
constexpr uint16_t k1 = 5;

// коэффициент входного напряжения
constexpr uint16_t input_k = k5;
// коэффициент для замера
constexpr uint16_t test_k = k1;

constexpr uint16_t target_rms_V = 230;  // amp 325
constexpr uint16_t target_rms_V_min = 150;  // amp 212
constexpr uint16_t target_rms_V_max = 315;  // amp 445

constexpr uint16_t target_rms_cV = target_rms_V * 100; // 230.00 В в сантивольтах

// коэффициент трансформации для любого выхода
constexpr float get_trans_factor(float k) {
  return k * target_rms_cV / input_k;
}

constexpr float test_to_in_factor = input_k / test_k;

// КПД трансформатора
constexpr float efficiency = 0.98;

// Падение на диоде выпрямления
constexpr float diode_drop = 0.613127;
constexpr float diode_drop_cV = diode_drop * 100;

// Характеристики делителm напряжения
constexpr float R1 = 42000;
constexpr float R2 = 10000;

constexpr float divider_factor = R1 / R2 + 1;

// Опорное напряжение и АЦП
constexpr uint16_t ADC_MAX_BIT = 10;
constexpr uint16_t VREF_V = 5;
constexpr uint16_t VREF_cV = VREF_V * 100; // 5 В в сантивольты

////////////////////////////////////////////////////////////////////////////////

// Пересчёт показаний АЦП в сантивольты на датчике
constexpr uint16_t adc_to_direct(uint16_t adc) {
  return (((uint32_t)adc * VREF_cV) >> ADC_MAX_BIT);
}

// Коэффициент от датчика до входа
constexpr float probe_to_in_factor = test_to_in_factor * divider_factor / efficiency;
constexpr uint16_t probe_to_in_factor_full = probe_to_in_factor * VREF_cV;

// Пересчёт АЦП в сантивольты на входе трансформатора
constexpr uint16_t adc_to_cV(uint16_t adc) {
  return ((uint32_t)(adc) * probe_to_in_factor_full) >> ADC_MAX_BIT;
}

// Пересчёт сантивольтов на входе трансформатора в показания АЦП
constexpr uint16_t cV_to_adc(uint16_t cV) {
  return ((uint32_t)cV << ADC_MAX_BIT) / probe_to_in_factor_full;
}

constexpr float lspace = 0.43;
constexpr float rspace = 0.57;

// Границы диапазонов по коэффициентам отвода
static constexpr Range<float> ranges_kt[4] = {
  Range<float>(0, utils::spacing(k4, k5, rspace)),
  Range<float>(utils::spacing(k4, k5, lspace), utils::spacing(k5, k6, rspace)),
  Range<float>(utils::spacing(k5, k6, lspace), utils::spacing(k6, k7, rspace)),
  Range<float>(utils::spacing(k6, k7, lspace), 0xFFFF),
};

// Границы диапазонов в сантивольтах (те же выражения, что в spacing_kt)
static constexpr Range<float> ranges_cV[4] = {
  ranges_kt[0].map<float>(get_trans_factor),
  ranges_kt[1].map<float>(get_trans_factor),
  ranges_kt[2].map<float>(get_trans_factor),
  ranges_kt[3].map<float>(get_trans_factor),
};

// Границы диапазонов в единицах АЦП
static constexpr Range<uint16_t> ranges_adc[4] = {
  ranges_cV[0].map<uint16_t>(cV_to_adc),
  ranges_cV[1].map<uint16_t>(cV_to_adc),
  ranges_cV[2].map<uint16_t>(cV_to_adc),
  ranges_cV[3].map<uint16_t>(cV_to_adc),
};

#endif