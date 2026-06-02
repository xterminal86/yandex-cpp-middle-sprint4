#include "metric_impl/cyclomatic_complexity.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

using namespace analyzer;
using namespace analyzer::metric;

TEST(CyclomaticComplexity, Test)
{
  std::unordered_map<std::string, size_t> testCases =
  {
      { "comments.py",         1 }
    , { "exceptions.py",       4 }
    , { "if.py",               2 }
    , { "loops.py",            4 }
    , { "many_lines.py",       2 }
    , { "many_parameters.py",  2 }
    , { "match_case.py",       4 }
    , { "nested_if.py",        4 }
    , { "simple.py",           2 }
    , { "ternary.py",          3 }
  };

  for (auto& kvp : testCases)
  {
    const std::string& fname   = kvp.first;
    const size_t expectedValue = kvp.second;

    MetricExtractor me;
    me.RegisterMetric<CyclomaticComplexityMetric>();

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
    me.RegisterMetric<CyclomaticComplexityMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "comments.py", "exceptions.py" },
      me
    );

    ASSERT_EQ(2, res.size());
    EXPECT_EQ(5, (res[0].second[0].value + res[1].second[0].value));
  }
}

}  // namespace analyzer::metric::metric_impl
