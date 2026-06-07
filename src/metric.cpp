#include "metric.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "function.hpp"

namespace analyzer::metric {

/**
 * @brief Вычисляет все зарегистрированные метрики для заданной функции.
 *
 * Эта функция применяет каждый метрический объект из контейнера `metrics`
 * к переданной функции `func` и собирает результаты в вектор.
 */
MetricResults MetricExtractor::Get(const function::Function& func) const
{
  // здесь ваш код
  MetricResults res;

  std::ranges::transform(
    metrics,
    std::back_inserter(res),
    [&func](auto& metric)
    {
      return metric->Calculate(func);
    }
  );

  return res;
}

}  // namespace analyzer::metric
