#include "metric_impl/naming_style.hpp"

#include "metric_accumulator_impl/categorical_accumulator.hpp"
#include "analyze.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

using namespace analyzer;
using namespace analyzer::metric;
using namespace analyzer::metric::metric_impl;

// здесь ваш код
TEST(CategoricalAccumulator, Test)
{
  MetricExtractor me;
  me.RegisterMetric<NamingStyleMetric>();

  AnalyzeResult res = AnalyzeFunctions(
    {
      "naming-style.py"
    },
    me
  );

  MetricsAccumulator accumulator;

  accumulator.RegisterAccumulator(NamingStyleMetric::kName,
                                  std::make_unique<CategoricalAccumulator>());

  AccumulateFunctionAnalysis(res, accumulator);

  auto& naming_style =
    accumulator.GetFinalizedAccumulator<CategoricalAccumulator>(
    NamingStyleMetric::kName
  );

  const auto& dict = naming_style.Get();

  EXPECT_EQ(1, dict.at("Unknown"));
  EXPECT_EQ(2, dict.at("Snake Case"));
  EXPECT_EQ(1, dict.at("Lower Case"));
  EXPECT_EQ(2, dict.at("Pascal Case"));
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
