#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <print>
#include <sstream>

#include "utils.hpp"

namespace analyzer::metric::metric_impl {

// =============================================================================

std::string CyclomaticComplexityMetric::Name() const
{
  return kName;
}

// =============================================================================

MetricResult::ValueType
CyclomaticComplexityMetric::CalculateImpl(const function::Function& f) const
{
  // Получаем строковое представление AST (абстрактного синтаксического дерева)
  // функции.
  // Это S-выражение, сгенерированное утилитой tree-sitter, например:
  // "(function_definition name: (identifier) ... (if_statement ...) (for_statement ...))"
  auto& function_ast = f.ast;

  // Список типов узлов AST, каждый из которых увеличивает цикломатическую
  // сложность на 1.
  // Эти узлы соответствуют управляющим конструкциям языка Python:
  // - if / elif
  // - циклы (for, while)
  // - обработка исключений (try, finally)
  // - case в match-выражениях
  // - assert
  // - тернарный оператор (conditional_expression)
  constexpr std::array<std::string_view, 9> complexity_nodes =
  {
      "if_statement"            // if
    , "elif_statement"          // elif
    , "for_statement"           // for
    , "while_statement"         // while
    , "try_statement"           // try
    , "finally_clause"          // finally
    , "case_clause"             // case
    , "assert"                  // assert
    , "conditional_expression"  // для тернарного оператора
  };

  // === ВАШ КОД ДОЛЖЕН БЫТЬ ЗДЕСЬ ===
  //
  // Цель: подсчитать, сколько раз в строке `function_ast` встречаются
  // любые из узлов из `complexity_nodes`.
  //
  // Важно:
  // - Имена узлов уникальны и не являются подстроками других имён, поэтому
  //   поиск подстроки (например, `"if_statement"`) безопасен.
  // - Каждое вхождение узла = +1 к сложности.
  // - В конце к общей сумме нужно прибавить 1 (базовая сложность функции без
  //   ветвлений).
  //
  // Пример:
  // Если AST содержит
  // "(if_statement ...) (for_statement ...) (if_statement ...)",
  // то найдено 3 узла → сложность = 3 + 1 = 4.
  //
  // Подсказка:
  // Можно пройтись по каждому `node_type` из `complexity_nodes` и подсчитать,
  // сколько раз он встречается в `function_ast`, используя `std::string::find`
  // в цикле (это допустимо, так как вы работаете со строковым представлением
  // AST, а не с исходным кодом напрямую).

  std::stringstream ss(function_ast);

  std::vector<std::string> lines;

  std::string line;
  while (std::getline(ss, line))
  {
    lines.push_back(line);
    //std::println("*** {}", line);
  }

  int total = 0;

  for (auto& what : complexity_nodes)
  {
    total += std::ranges::count_if(
      lines,
      [&](const std::string& line)
      {
        size_t pos = line.find(what);

        bool found = (pos != std::string::npos);
        //if (found)
        //{
        //  std::println("{} *** found '{}'", __FILE__, what);
        //}

        return found;
      }
    );
  }

  total++;

  return total;
}
}  // namespace analyzer::metric::metric_impl
