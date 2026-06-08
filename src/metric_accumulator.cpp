#include "metric_accumulator.hpp"

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
#include <print>

namespace analyzer::metric_accumulator {
/**
 * @brief Накапливает результаты метрик для одной функции.
 *
 * Эта функция принимает вектор результатов метрик (например,
 * "цикломатическая сложность = 4", "число строк = 12", "параметров = 3") и
 * передаёт каждый результат соответствующему аккумулятору.
 *
 * Как это работает:
 * - Для каждого `metric_result` из `metric_results` извлекается имя метрики
 *   (`metric_name`).
 * - По этому имени в контейнере `accumulators` находится нужный аккумулятор.
 * - Вызывается метод `Accumulate(metric_result)`, который обновляет внутреннее
 *   состояние аккумулятора.
 */
void MetricsAccumulator::AccumulateNextFunctionResults(
  const std::vector<metric::MetricResult>& metric_results
) const
{
  // здесь ваш код
  std::ranges::for_each(
    metric_results,
    [&](auto& m)
    {
      if (accumulators.count(m.metric_name) == 1)
      {
        accumulators.at(m.metric_name)->Accumulate(m);
      }
      else
      {
        std::println("{}:{} - nothing to do for metric name '{}'",
                   __FILE__, __LINE__, m.metric_name);
      }
    }
  );
}

/**
 * @brief Сбрасывает состояние всех аккумуляторов.
 *
 * Эта функция вызывается, когда нужно начать новую агрегацию (например, для
 * нового файла или класса).
 * Она проходит по всем зарегистрированным аккумуляторам и вызывает у каждого
 * метод `Reset()`, который обнуляет накопленные значения (сумму, счётчик и т.д.).
 */
void MetricsAccumulator::ResetAccumulators()
{
  // здесь ваш код
  std::ranges::for_each(
    accumulators | std::views::values,
    [](auto& a)
    {
      a->Reset();
    }
  );
}

}  // namespace analyzer::metric_accumulator
