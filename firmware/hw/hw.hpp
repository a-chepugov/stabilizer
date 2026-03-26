#pragma once

#include "adc.hpp"
#include "transformer.hpp"

namespace hw {
  // допустимое отклонение +10% / -15%

  constexpr uint16_t OUTPUT_cV_RMS_TARGET = 220 * 100; // amp 311
  constexpr uint16_t OUTPUT_cV_RMS_MIN = OUTPUT_cV_RMS_TARGET * 0.85; 
  constexpr uint16_t OUTPUT_cV_RMS_MAX = OUTPUT_cV_RMS_TARGET * 1.1;

  constexpr uint16_t INPUT_cV_RMS_MIN = tr::calc_input_voltage(OUTPUT_cV_RMS_MIN, tr::k_in_default, tr::k_7);
  constexpr uint16_t INPUT_cV_RMS_MAX = tr::calc_input_voltage(OUTPUT_cV_RMS_MAX, tr::k_in_default, tr::k_4);
  
  /**
  | # |   k | amp | rms | rms|t |
  | - | --- | --- | --- | ----- |
  | 7 | 100 | 249 | 176 |   354 | 
  | 6 |  90 | 276 | 195 |   394 |
  | 5 |  80 | 311 | 220 |   448 |
  | 4 |  65 | 382 | 270 |   554 |
  */

  constexpr float k_crit_hi = tr::calc_output_k(INPUT_cV_RMS_MAX, OUTPUT_cV_RMS_TARGET, tr::k_in_default);
  constexpr float k_crit_lo = tr::calc_output_k(INPUT_cV_RMS_MIN, OUTPUT_cV_RMS_TARGET, tr::k_in_default);

  constexpr float test_to_in_factor = tr::k_in_default / tr::test_k;
  constexpr uint16_t i_test_to_in_factor = test_to_in_factor;
  
  // Падение на диоде выпрямления
  constexpr float diode_drop = 0.613127;
  constexpr uint16_t diode_drop_cV = diode_drop * 100;

  // Характеристики делителm напряжения
  constexpr float R1 = 50000;
  constexpr float R2 = 10000;

  constexpr float divider_factor = R1 / R2 + 1;

  ////////////////////////////////////////////////////////////////////////////////

  // Коэффициент от датчика до входа
  constexpr uint16_t probe_to_divider_factor = (float)adc::VREF_cV * divider_factor * adc::LEVELS / adc::MAX_VALUE;

  // Пересчёт показаний АЦП в сантивольты на входе трансформатора
  constexpr uint16_t adc_to_cV(uint16_t input) {
    return (
      (uint16_t)(
        ((uint32_t)input * probe_to_divider_factor) >> adc::BITS
      ) + diode_drop_cV
    ) * i_test_to_in_factor
    ;
  }

  // Пересчёт сантивольтов на выходе трансформатора в показания АЦП
  constexpr float V_to_adc(float V) {
    return (V / test_to_in_factor - diode_drop) / divider_factor / adc::VREF * adc::MAX_VALUE;
  }

  // Пересчёт сантивольтов на выходе трансформатора в показания АЦП
  constexpr uint16_t cV_to_adc_fast(uint16_t cV) {
    return (
      (uint32_t)(
        cV / i_test_to_in_factor - diode_drop_cV
      ) << adc::BITS
    ) / probe_to_divider_factor;
  }

}