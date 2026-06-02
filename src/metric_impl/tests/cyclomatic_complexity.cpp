#include "metric_impl/cyclomatic_complexity.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

using namespace analyzer;
using namespace analyzer::metric;

TEST(CyclomaticComplexity, Test)
{
  {
    MetricExtractor me;
    me.RegisterMetric<CyclomaticComplexityMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "comments.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(1, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CyclomaticComplexityMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "exceptions.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(4, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CyclomaticComplexityMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "nested_if.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(4, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CyclomaticComplexityMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "ternary.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(3, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CyclomaticComplexityMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "loops.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(4, res[0].second[0].value);
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
