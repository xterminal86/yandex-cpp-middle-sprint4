#include "metric_impl/code_lines_count.hpp"
#include "metric_impl/cyclomatic_complexity.hpp"
#include "metric_impl/parameters_count.hpp"

#include "metric_accumulator_impl/average_accumulator.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

using namespace analyzer;
using namespace analyzer::metric;
using namespace analyzer::metric::metric_impl;

// здесь ваш код
TEST(AverageAccumulator, Test)
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
                                  std::make_unique<AverageAccumulator>());
  accumulator.RegisterAccumulator(CodeLinesCountMetric::kName,
                                  std::make_unique<AverageAccumulator>());
  accumulator.RegisterAccumulator(CountParametersMetric::kName,
                                  std::make_unique<AverageAccumulator>());

  AccumulateFunctionAnalysis(res, accumulator);

  auto& cc_average =
    accumulator.GetFinalizedAccumulator<AverageAccumulator>(
    CyclomaticComplexityMetric::kName
  );

  auto& cl_average =
    accumulator.GetFinalizedAccumulator<AverageAccumulator>(
    CodeLinesCountMetric::kName
  );

  auto& cp_average =
    accumulator.GetFinalizedAccumulator<AverageAccumulator>(
    CountParametersMetric::kName
  );

  EXPECT_DOUBLE_EQ(2.8, cc_average.Get());
  EXPECT_DOUBLE_EQ(5.2, cl_average.Get());
  EXPECT_DOUBLE_EQ(1.4, cp_average.Get());
}

// =============================================================================

TEST(AverageAccumulator, Empty)
{
  MetricExtractor me;
  me.RegisterMetric<CyclomaticComplexityMetric>();
  me.RegisterMetric<CodeLinesCountMetric>();
  me.RegisterMetric<CountParametersMetric>();

  AnalyzeResult res = AnalyzeFunctions({}, me);

  MetricsAccumulator accumulator;

  accumulator.RegisterAccumulator(CyclomaticComplexityMetric::kName,
                                  std::make_unique<AverageAccumulator>());
  accumulator.RegisterAccumulator(CodeLinesCountMetric::kName,
                                  std::make_unique<AverageAccumulator>());
  accumulator.RegisterAccumulator(CountParametersMetric::kName,
                                  std::make_unique<AverageAccumulator>());

  AccumulateFunctionAnalysis(res, accumulator);

  auto& cc_average =
    accumulator.GetFinalizedAccumulator<AverageAccumulator>(
    CyclomaticComplexityMetric::kName
  );

  auto& cl_average =
    accumulator.GetFinalizedAccumulator<AverageAccumulator>(
    CodeLinesCountMetric::kName
  );

  auto& cp_average =
    accumulator.GetFinalizedAccumulator<AverageAccumulator>(
    CountParametersMetric::kName
  );

  EXPECT_DOUBLE_EQ(0.0, cc_average.Get());
  EXPECT_DOUBLE_EQ(0.0, cl_average.Get());
  EXPECT_DOUBLE_EQ(0.0, cp_average.Get());
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
