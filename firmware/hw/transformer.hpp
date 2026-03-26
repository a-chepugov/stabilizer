#pragma once

namespace tr {

  // Коэффициенты преобразования трансформатора (отводы)
  constexpr uint16_t k_7 = 100;
  constexpr uint16_t k_6 = 90;
  constexpr uint16_t k_5 = 80;
  constexpr uint16_t k_4 = 65;
  constexpr uint16_t k_3 = 6;
  constexpr uint16_t k_2 = 5;

  // Коэффициент входного напряжения
  constexpr uint16_t k_in_default = k_5;
  // Коэффициент для замера
  constexpr uint16_t test_k = k_2;

  // Расчет выходного напряжения
  constexpr float calc_output_voltage(float V_in, float k_in, float k_out) {
    return V_in * k_out / k_in;
  }

  // Расчет входного напряжения
  constexpr float calc_input_voltage(float V_out, float k_in, float k_out) {
    return V_out * k_in / k_out;
  }

  // Расчет коэффициента
  constexpr float calc_output_k(float V_in, float V_out, float k_in) {
    return V_out * k_in / V_in;
  }
}
