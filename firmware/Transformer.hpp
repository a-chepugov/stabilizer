#pragma once

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
constexpr uint16_t i_test_to_in_factor = test_to_in_factor;

// Падение на диоде выпрямления
constexpr float diode_drop = 0.613127;
constexpr uint16_t diode_drop_cV = diode_drop * 100;

// Характеристики делителm напряжения
constexpr float R1 = 42000;
constexpr float R2 = 10000;

constexpr float divider_factor = R1 / R2 + 1;

// Опорное напряжение и АЦП
constexpr uint16_t ADC_BITS = 10;
constexpr uint16_t ADC_LEVELS = 1UL << ADC_BITS;
constexpr uint16_t ADC_MAX_VALUE = ADC_LEVELS - 1;

constexpr uint16_t VREF_cV = 500; // 5.00 В в сантивольтах

////////////////////////////////////////////////////////////////////////////////

// Пересчёт показаний АЦП в сантивольты на датчике
constexpr uint16_t adc_to_direct(uint16_t adc) {
  return (((uint32_t)adc * VREF_cV) / ADC_MAX_VALUE);
}

// Коэффициент от датчика до входа
constexpr uint16_t probe_to_divider_factor = (float)VREF_cV * divider_factor * ADC_LEVELS / ADC_MAX_VALUE;

// Пересчёт АЦП в сантивольты на входе трансформатора
constexpr uint16_t adc_to_cV(uint16_t adc) {
  return (
    (uint16_t)(
      ((uint32_t)adc * probe_to_divider_factor) >> ADC_BITS
    ) + diode_drop_cV
  ) * i_test_to_in_factor
  ;
}

// Пересчёт сантивольтов на входе трансформатора в показания АЦП
constexpr uint16_t cV_to_adc(uint16_t cV) {
  return ((uint32_t)(cV / i_test_to_in_factor - diode_drop_cV) << ADC_BITS) / probe_to_divider_factor;
}