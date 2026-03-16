#pragma once

#include "Transformer.hpp"
#include "utils.hpp"
#include "ConstArray.hpp"

namespace cfg {
  constexpr float shift = 0.05;
  constexpr float lpart = 0.5 - shift;
  constexpr float rpart = 0.5 + shift;

  struct TLineCfg { uint16_t l; uint16_t r; float part; };

  constexpr TLineCfg t4h = { k4, k5, rpart };
  constexpr TLineCfg t5l = { k4, k5, lpart };
  constexpr TLineCfg t5h = { k5, k6, rpart };
  constexpr TLineCfg t6l = { k5, k6, lpart };
  constexpr TLineCfg t6h = { k6, k7, rpart };
  constexpr TLineCfg t7l = { k6, k7, lpart };

  constexpr ConstArray<TLineCfg, 6> t_k_tuples = {{
    t4h,
    t5l,
    t5h,
    t6l,
    t6h,
    t7l,
  }};

  constexpr float place (TLineCfg cfg) { 
    return utils::spacing(cfg.l, cfg.r, cfg.part); 
  };

  // "Границы" диапазонов по коэффициентам отвода
  constexpr ConstArray<float, 6>k_ranges = t_k_tuples.map<float>(place);
  
  // Границы диапазонов в сантивольтах
  constexpr ConstArray<float, 6>cV_ranges = k_ranges.map<float>(get_trans_factor);
  // Границы диапазонов в единицах АЦП
  constexpr ConstArray<uint16_t, 6>adc_ranges = cV_ranges.map<uint16_t>(cV_to_adc);
}