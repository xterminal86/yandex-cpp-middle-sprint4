#include "metric_impl/parameters_count.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

TEST(ParametersCount, Test)
{
  {
    MetricExtractor me;
    me.RegisterMetric<CountParametersMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "many_parameters.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(5, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CountParametersMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "simple.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(0, res[0].second[0].value);
  }
}

}  // namespace analyzer::metric::metric_impl
