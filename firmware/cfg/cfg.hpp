#pragma once
#include <stdint.h>

#include "Transformer.hpp"
#include "../utils/utils.hpp"
#include "../models/ArrayView.hpp"

namespace cfg {
  constexpr float shift = 0.05;
  constexpr float lpart = 0.5 - shift;
  constexpr float rpart = 0.5 + shift;

  struct TLineCfg { uint16_t a; uint16_t b; float part; };

  constexpr TLineCfg t_min = { tr::k_crit_min, tr::k_4, 0 + shift };
  constexpr TLineCfg t_4_l = { tr::k_crit_min, tr::k_4, 0 };
  constexpr TLineCfg t_4_h = { tr::k_4, tr::k_5, rpart };
  constexpr TLineCfg t_5_l = { tr::k_4, tr::k_5, lpart };
  constexpr TLineCfg t_5_h = { tr::k_5, tr::k_6, rpart };
  constexpr TLineCfg t_6_l = { tr::k_5, tr::k_6, lpart };
  constexpr TLineCfg t_6_h = { tr::k_6, tr::k_7, rpart };
  constexpr TLineCfg t_7_l = { tr::k_6, tr::k_7, lpart };
  constexpr TLineCfg t_7_h = { tr::k_7, tr::k_crit_max, 1 };
  constexpr TLineCfg t_max = { tr::k_7, tr::k_crit_max, 1 - shift };

  constexpr ArrayView<TLineCfg, 10> t_k_tuples{{
    t_min,
    t_4_l,
    t_4_h,
    t_5_l,
    t_5_h,
    t_6_l,
    t_6_h,
    t_7_l,
    t_7_h,
    t_max,
  }};

  constexpr float place(TLineCfg cfg) {
    return utils::spacing(cfg.a, cfg.b, cfg.part);
  };

  // "Границы" диапазонов по коэффициентам отвода
  constexpr ArrayView<float, 10>t_k_ranges = t_k_tuples.map<float>(place);

  // Границы диапазонов в сантивольтах
  constexpr ArrayView<float, 10>cV_ranges = t_k_ranges.map<float>(tr::get_input_cV);

  // Границы диапазонов в единицах АЦП
  constexpr ArrayView<uint16_t, 10>adc_ranges = cV_ranges.map<uint16_t>(tr::cV_to_adc);
}
