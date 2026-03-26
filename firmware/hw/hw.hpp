#pragma once

#include "adc.hpp"
#include "transformer.hpp"

namespace hw {
  // допустимое отклонение +10% / -15%

  constexpr float OUTPUT_V_RMS_TARGET = 220; // amp 311
  constexpr float OUTPUT_V_RMS_MIN = OUTPUT_V_RMS_TARGET * 0.85; 
  constexpr float OUTPUT_V_RMS_MAX = OUTPUT_V_RMS_TARGET * 1.1;
  
  constexpr uint16_t OUTPUT_cV_RMS_TARGET = OUTPUT_V_RMS_TARGET * 100; // amp 311
  constexpr uint16_t OUTPUT_cV_RMS_MIN = OUTPUT_V_RMS_MIN * 100; 
  constexpr uint16_t OUTPUT_cV_RMS_MAX = OUTPUT_V_RMS_MAX * 100;


  constexpr float INPUT_V_RMS_MIN = tr::calc_input_voltage(OUTPUT_V_RMS_MIN, tr::k_in_default, tr::k_7);
  constexpr float INPUT_V_RMS_MAX = tr::calc_input_voltage(OUTPUT_V_RMS_MAX, tr::k_in_default, tr::k_4);

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

  constexpr float k_crit_hi = tr::calc_output_k(INPUT_V_RMS_MAX, OUTPUT_V_RMS_TARGET, tr::k_in_default);
  constexpr float k_crit_lo = tr::calc_output_k(INPUT_V_RMS_MIN, OUTPUT_V_RMS_TARGET, tr::k_in_default);

  constexpr float test_to_in_factor = tr::k_in_default / tr::test_k;
  
  // Падение на диоде выпрямления
  constexpr float diode_drop = 0.613127;
  constexpr uint16_t diode_drop_cV = diode_drop * 100;

  // Характеристики делителm напряжения
  constexpr float R1 = 50000;
  constexpr float R2 = 10000;

  constexpr float divider_factor = R1 / R2 + 1;

  ////////////////////////////////////////////////////////////////////////////////

  // Пересчёт показаний АЦП в сантивольты на входе трансформатора
  constexpr float adc_to_V(uint16_t input) {
    if (input > 0) {
      return (
        adc::to_V(input) * divider_factor + diode_drop
      ) * test_to_in_factor
      ;
    } else {
      return 0;
    }
  }

  // Пересчёт сантивольтов на выходе трансформатора в показания АЦП
  constexpr uint16_t V_to_adc(float V) {
    if (V > 0) {
      return (V / test_to_in_factor - diode_drop) / divider_factor / adc::VREF * adc::MAX_VALUE;
    } else {
      return 0;
    }
  }

  // Коэффициенты для целочисленных вычислений
  constexpr uint16_t probe_to_divider_factor = (float)adc::VREF_cV * divider_factor * adc::LEVELS / adc::MAX_VALUE;
  constexpr uint16_t i_test_to_in_factor = test_to_in_factor;

  // Пересчёт показаний АЦП в сантивольты на входе трансформатора (целочисленный варианти)
  constexpr uint16_t adc_to_cV(uint16_t input) {
    if (input > 0) {
      return (
        (uint16_t)(
          ((uint32_t)input * probe_to_divider_factor) >> adc::BITS
        ) + diode_drop_cV
      ) * i_test_to_in_factor
      ;
    } else {
      return 0;
    }
  }

  // Пересчёт сантивольтов на выходе трансформатора в показания АЦП (целочисленный вариант)
  constexpr uint16_t cV_to_adc(uint16_t cV) {
    if (cV >0) {
      return (
        (uint32_t)(
          cV / i_test_to_in_factor - diode_drop_cV
        ) << adc::BITS
      ) / probe_to_divider_factor;
    } else {
      return 0;
    }
  }

}