#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <typeinfo>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyzer {

namespace rv = std::ranges::views;
namespace rs = std::ranges;
/**
 * @brief Анализирует список Python-файлов и извлекает метрики для всех функций
 * и методов.
 *
 * Эта функция — центральный "конвейер" обработки:
 * 1. Принимает имена файлов.
 * 2. Для каждого файла создаёт объект `File`, который автоматически парсит его
 *    через tree-sitter и строит AST.
 * 3. Извлекает из AST все функции и методы с помощью `FunctionExtractor`.
 * 4. Объединяет все функции из всех файлов в один плоский список (`join`).
 * 5. Для каждой функции вычисляет набор метрик через переданный
 *    `metric_extractor`.
 * 6. Возвращает вектор пар: (функция, результаты её метрик).
 */
using PairFnRes = std::pair<function::Function, metric::MetricResults>;
using AnalyzeResult = std::vector<PairFnRes>;

//
// NOTE: need inline for tests to compile.
//
inline AnalyzeResult AnalyzeFunctions(
  const std::vector<std::string>& files,
  const metric::MetricExtractor& metric_extractor
)
{
  // здесь ваш код
  auto stuff = files | std::views::transform(
    [&](auto& fname)
    {
      function::FunctionExtractor fe;
      return fe.Get(fname);
    }
  )
  | std::views::join
  | std::views::transform(
    [&](const function::Function& fn)
    {
      return std::pair{ fn, metric_extractor.Get(fn) };
    }
  );

  return stuff | std::ranges::to<AnalyzeResult>();
}

/**
 *
 * @brief Группирует результаты анализа по классам.
 *
 * Эта функция:
 * 1. Отфильтровывает только те функции, которые являются **методами классов**
 *    (у них `class_name.has_value()` == true).
 * 2. Группирует последовательные элементы с одинаковым именем класса с помощью
 *    `chunk_by`.
 *
 * Важно:
 * - `chunk_by` работает только с **последовательными** одинаковыми элементами!
 *   Поэтому предполагается, что входной диапазон уже упорядочен по классам
 *   (например, порядок методов в AST сохраняется как в исходном файле).
 * - Если порядок нарушен, один и тот же класс может быть разбит на несколько
 *   групп.
 *
 *  Чтобы убедиться, что фильтрация работает, проверьте, что свободные функции
 *  (без class_name) действительно исчезают из результата.
 */
inline auto SplitByClasses(const AnalyzeResult& analysis)
{
  // здесь ваш код
  return analysis
    | std::views::filter(
      [](const auto& p)
      {
        const function::Function fn = p.first;
        return fn.class_name.has_value();
      }
    )
    | std::views::chunk_by(
      [](const auto& p1, const auto& p2)
      {
        const function::Function fn1 = p1.first;
        const function::Function fn2 = p2.first;

        return fn1.class_name == fn2.class_name;
      }
  );
}

/**
 * @brief Группирует результаты анализа по исходным файлам.
 *
 * Эта функция:
 * - Разбивает весь список функций на группы, где каждая группа содержит
 *   только функции из одного и того же файла (`filename`).
 * - Использует `chunk_by`, поэтому **порядок функций в `analysis` должен быть
 *   по файлам**.
 */
inline auto SplitByFiles(const AnalyzeResult& analysis)
{
  // здесь ваш код
  return analysis
    | std::views::chunk_by(
      [](const auto& a, const auto& b)
      {
        return a.first.filename == b.first.filename;
      }
    )
    | std::ranges::to<std::vector>();
}

/**
 * @brief Агрегирует метрики всех функций с помощью аккумулятора.
 *
 * Эта функция:
 * - Проходит по каждому элементу результата `AnalyzeFunctions`
 *   (то есть по каждой функции и её метрикам).
 * - Передаёт результаты метрик (`elem.second`) в аккумулятор через
 *   `AccumulateNextFunctionResults`.
 */
inline void AccumulateFunctionAnalysis(
  const AnalyzeResult& analysis,
  const analyzer::metric_accumulator::MetricsAccumulator& accumulator
)
{
  // здесь ваш код
  std::ranges::for_each(
    analysis,
    [&accumulator](const auto& p)
    {
      accumulator.AccumulateNextFunctionResults(p.second);
    }
  );
}

}  // namespace analyzer
