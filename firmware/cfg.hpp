#pragma once

#include "Transformer.hpp"
#include "utils.hpp"
#include "ConstArray.hpp"

namespace cfg {
  constexpr float shift = 0.05;
  constexpr float lpart = 0.5 - shift;
  constexpr float rpart = 0.5 + shift;

  struct TLineCfg { uint16_t l; uint16_t r; float part; };

  constexpr TLineCfg t_min = { k_crit_min, k_4, 0 + shift };
  constexpr TLineCfg t_4_l = { k_crit_min, k_4, 0 };
  constexpr TLineCfg t_4_h = { k_4, k_5, rpart };
  constexpr TLineCfg t_5_l = { k_4, k_5, lpart };
  constexpr TLineCfg t_5_h = { k_5, k_6, rpart };
  constexpr TLineCfg t_6_l = { k_5, k_6, lpart };
  constexpr TLineCfg t_6_h = { k_6, k_7, rpart };
  constexpr TLineCfg t_7_l = { k_6, k_7, lpart };
  constexpr TLineCfg t_7_h = { k_7, k_crit_max, 1 };
  constexpr TLineCfg t_max = { k_7, k_crit_max, 1 - shift };

  constexpr ConstArray<TLineCfg, 10> t_k_tuples = {{
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

  constexpr float place (TLineCfg cfg) { 
    return utils::spacing(cfg.l, cfg.r, cfg.part); 
  };

  // "Границы" диапазонов по коэффициентам отвода
  constexpr ConstArray<float, 10>t_k_ranges = t_k_tuples.map<float>(place);
  
  // Границы диапазонов в сантивольтах
  constexpr ConstArray<float, 10>cV_ranges = t_k_ranges.map<float>(get_input_cV);

  // Границы диапазонов в единицах АЦП
  constexpr ConstArray<uint16_t, 10>adc_ranges = cV_ranges.map<uint16_t>(cV_to_adc);
}