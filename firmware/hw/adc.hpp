#pragma once

namespace adc {
  // разрядность АЦП
  constexpr uint16_t BITS = 10;
  constexpr uint16_t LEVELS = 1UL << BITS;
  constexpr uint16_t MAX_VALUE = LEVELS - 1;

  // Максимальное напряжение на АЦП в сантивольнах
  constexpr float VREF = 5.0;
  constexpr uint16_t VREF_cV = VREF * 100;

  // Пересчёт показаний АЦП в сантивольты на датчике
  constexpr uint16_t to_cV(uint16_t input) {
    return (((uint32_t)input * VREF_cV) / MAX_VALUE);
  }
}