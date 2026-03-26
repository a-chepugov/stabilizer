#pragma once

#include "../hw/adc.hpp"
#include "../hw/transformer.hpp"

namespace tr {
  constexpr uint16_t target_rms_V = 220;  // amp 311
  constexpr uint16_t target_rms_V_min = 155;  // amp 219
  constexpr uint16_t target_rms_V_max = 315;  // amp 445

  /**
  | # |   k | rms | amp | rms|t |
  | - | --- | --- | --- | ----- |
  | 7 | 100 | 176 | 249 |   354 | 
  | 6 |  90 | 195 | 276 |   394 |
  | 5 |  80 | 220 | 311 |   448 |
  | 4 |  65 | 270 | 382 |   554 |
  */

  constexpr uint16_t target_rms_cV = target_rms_V * 100; // 230.00 В в сантивольтах

  constexpr float get_trans_koef(float cV) {
    return (float)target_rms_cV * input_k / cV;
  }

  constexpr uint16_t k_crit_min = get_trans_koef(target_rms_V_max * 100);
  constexpr uint16_t k_crit_max = get_trans_koef(target_rms_V_min * 100);

  constexpr float test_to_in_factor = input_k / test_k;
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

  // Пересчёт сантивольтов на входе трансформатора в показания АЦП
  constexpr uint16_t cV_to_adc(uint16_t cV) {
    return (
      (uint32_t)(
        cV / i_test_to_in_factor - diode_drop_cV
      ) << adc::BITS
    ) / probe_to_divider_factor;
  }

}