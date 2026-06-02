#include "metric_impl/code_lines_count.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

using namespace analyzer;
using namespace analyzer::metric;

// здесь ваш код
TEST(CodeLinesCount, Test)
{
  std::unordered_map<std::string, size_t> testCases =
  {
      { "comments.py",         3 }
    , { "exceptions.py",       7 }
    , { "if.py",               3 }
    , { "loops.py",            6 }
    , { "many_lines.py",      11 }
    , { "many_parameters.py",  1 }
    , { "match_case.py",       7 }
    , { "nested_if.py",        8 }
    , { "simple.py",           5 }
    , { "ternary.py",          1 }
  };

  for (auto& kvp : testCases)
  {
    const std::string& fname   = kvp.first;
    const size_t expectedValue = kvp.second;

    MetricExtractor me;
    me.RegisterMetric<CodeLinesCountMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { fname },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(expectedValue, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CodeLinesCountMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "comments.py", "exceptions.py" },
      me
    );

    ASSERT_EQ(2, res.size());
    EXPECT_EQ(3, res[0].second[0].value);
    EXPECT_EQ(7, res[1].second[0].value);
  }
}

}  // namespace analyzer::metric::metric_impl

