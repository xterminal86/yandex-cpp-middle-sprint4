#include "metric_impl/code_lines_count.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

using namespace analyzer;
using namespace analyzer::metric;

// здесь ваш код
TEST(CodeLinesCount, Test)
{
  {
    MetricExtractor me;
    me.RegisterMetric<CodeLinesCountMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "comments.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(3, res[0].second[0].value);
  }
  // ---------------------------------------------------------------------------
  {
    MetricExtractor me;
    me.RegisterMetric<CodeLinesCountMetric>();

    AnalyzeResult res = AnalyzeFunctions(
      { "exceptions.py" },
      me
    );

    ASSERT_EQ(1, res.size());
    EXPECT_EQ(7, res[0].second[0].value);
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
    EXPECT_EQ(10, (res[0].second[0].value + res[1].second[0].value));
  }
}

}  // namespace analyzer::metric::metric_impl
