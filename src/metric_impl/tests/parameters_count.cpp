#include "metric_impl/parameters_count.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

TEST(ParametersCount, Test)
{
  std::unordered_map<std::string, size_t> testCases =
  {
      { "comments.py",         3 }
    , { "exceptions.py",       0 }
    , { "if.py",               1 }
    , { "loops.py",            1 }
    , { "many_lines.py",       0 }
    , { "many_parameters.py",  5 }
    , { "match_case.py",       1 }
    , { "nested_if.py",        2 }
    , { "simple.py",           0 }
    , { "ternary.py",          1 }
  };

  for (auto& kvp : testCases)
  {
    const std::string& fname   = kvp.first;
    const size_t expectedValue = kvp.second;

    MetricExtractor me;
    me.RegisterMetric<CountParametersMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { fname },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(expectedValue, res[0].second[0].value);
  }
}

}  // namespace analyzer::metric::metric_impl
