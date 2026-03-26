#pragma once
#include <stdint.h>

#include "../hw/hw.hpp"
#include "../utils/utils.hpp"
#include "../models/ArrayView.hpp"

namespace cfg {
  constexpr float shift = 0.05;
  constexpr float lpart = 0.5 - shift;
  constexpr float rpart = 0.5 + shift;

  struct TLineCfg { float a; float b; float part; };

  constexpr TLineCfg t_max = { hw::k_crit_hi, tr::k_4, 0 + shift };
  constexpr TLineCfg t_4_l = { hw::k_crit_hi, tr::k_4, 0 };
  constexpr TLineCfg t_4_h = { tr::k_4, tr::k_5, rpart };
  constexpr TLineCfg t_5_l = { tr::k_4, tr::k_5, lpart };
  constexpr TLineCfg t_5_h = { tr::k_5, tr::k_6, rpart };
  constexpr TLineCfg t_6_l = { tr::k_5, tr::k_6, lpart };
  constexpr TLineCfg t_6_h = { tr::k_6, tr::k_7, rpart };
  constexpr TLineCfg t_7_l = { tr::k_6, tr::k_7, lpart };
  constexpr TLineCfg t_7_h = { tr::k_7, hw::k_crit_lo, 1 };
  constexpr TLineCfg t_min = { tr::k_7, hw::k_crit_lo, 1 - shift };

  constexpr ArrayView<TLineCfg, 10> t_k_tuples{{
    t_max,
    t_4_l,
    t_4_h,
    t_5_l,
    t_5_h,
    t_6_l,
    t_6_h,
    t_7_l,
    t_7_h,
    t_min,
  }};

  constexpr float place(TLineCfg cfg) {
    return utils::spacing(cfg.a, cfg.b, cfg.part);
  };

  constexpr float get_input_V(float k_out) {
    return tr::calc_input_voltage(hw::OUTPUT_V_RMS_TARGET, tr::k_in_default, k_out);
  }

  // "Границы" диапазонов по коэффициентам отвода
  constexpr ArrayView<float, 10>t_k_ranges = t_k_tuples.map<float>(place);

  // Границы диапазонов в сантивольтах
  constexpr ArrayView<float, 10>V_ranges = t_k_ranges.map<float>(get_input_V);

  // Границы диапазонов в единицах АЦП
  constexpr ArrayView<uint16_t, 10>adc_ranges = V_ranges.map<uint16_t>(hw::V_to_adc);
}
