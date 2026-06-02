#include "metric_impl/code_lines_count.hpp"
#include "metric_impl/cyclomatic_complexity.hpp"
#include "metric_impl/parameters_count.hpp"

#include "metric_accumulator_impl/sum_average_accumulator.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

using namespace analyzer;
using namespace analyzer::metric;
using namespace analyzer::metric::metric_impl;

TEST(SumAverageAccumulator, Test)
{
  MetricExtractor me;
  me.RegisterMetric<CyclomaticComplexityMetric>();
  me.RegisterMetric<CodeLinesCountMetric>();
  me.RegisterMetric<CountParametersMetric>();

  AnalyzeResult res = AnalyzeFunctions(
    {
      "comments.py"
    , "exceptions.py"
    , "if.py"
    , "loops.py"
    , "many_lines.py"
    , "many_parameters.py"
    , "match_case.py"
    , "nested_if.py"
    , "simple.py"
    , "ternary.py"
    },
    me
  );

  MetricsAccumulator accumulator;

  accumulator.RegisterAccumulator(CyclomaticComplexityMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());
  accumulator.RegisterAccumulator(CodeLinesCountMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());
  accumulator.RegisterAccumulator(CountParametersMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());

  AccumulateFunctionAnalysis(res, accumulator);

  auto& cc_saverage =
    accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
    CyclomaticComplexityMetric::kName
  );

  auto& cl_saverage =
    accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
    CodeLinesCountMetric::kName
  );

  auto& cp_saverage =
    accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
    CountParametersMetric::kName
  );

  EXPECT_EQ(28, cc_saverage.Get().sum);
  EXPECT_DOUBLE_EQ(2.8, cc_saverage.Get().average);

  EXPECT_EQ(52, cl_saverage.Get().sum);
  EXPECT_DOUBLE_EQ(5.2, cl_saverage.Get().average);

  EXPECT_EQ(14, cp_saverage.Get().sum);
  EXPECT_DOUBLE_EQ(1.4, cp_saverage.Get().average);
}

// =============================================================================

TEST(SumAverageAccumulator, Empty)
{
  MetricExtractor me;
  me.RegisterMetric<CyclomaticComplexityMetric>();
  me.RegisterMetric<CodeLinesCountMetric>();
  me.RegisterMetric<CountParametersMetric>();

  AnalyzeResult res = AnalyzeFunctions({}, me);

  MetricsAccumulator accumulator;

  accumulator.RegisterAccumulator(CyclomaticComplexityMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());
  accumulator.RegisterAccumulator(CodeLinesCountMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());
  accumulator.RegisterAccumulator(CountParametersMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());

  AccumulateFunctionAnalysis(res, accumulator);

  auto& cc_saverage =
    accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
    CyclomaticComplexityMetric::kName
  );

  auto& cl_saverage =
    accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
    CodeLinesCountMetric::kName
  );

  auto& cp_saverage =
    accumulator.GetFinalizedAccumulator<SumAverageAccumulator>(
    CountParametersMetric::kName
  );

  EXPECT_EQ(0, cc_saverage.Get().sum);
  EXPECT_DOUBLE_EQ(0.0, cc_saverage.Get().average);

  EXPECT_EQ(0, cl_saverage.Get().sum);
  EXPECT_DOUBLE_EQ(0.0, cl_saverage.Get().average);

  EXPECT_EQ(0, cp_saverage.Get().sum);
  EXPECT_DOUBLE_EQ(0.0, cp_saverage.Get().average);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
