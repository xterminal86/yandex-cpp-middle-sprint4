#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "analyze.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

int main(int argc, char *argv[])
{
  using namespace analyzer;

  cmd::ProgramOptions options;
  if (!options.Parse(argc, argv))
  {
    return 1;
  }

  using namespace analyzer::metric;
  using namespace analyzer::metric::metric_impl;
  using namespace analyzer::metric_accumulator;

  MetricExtractor metric_extractor;

  metric_extractor.RegisterMetric<CyclomaticComplexityMetric>();
  metric_extractor.RegisterMetric<CodeLinesCountMetric>();

  // По условию это является опциональным, так что не будем делать.
  //metric_extractor.RegisterMetric<NamingStyleMetric>();

  metric_extractor.RegisterMetric<CountParametersMetric>();

  AnalyzeResult analysis = AnalyzeFunctions(
    options.GetFiles(),
    metric_extractor
  );

  std::println("Analysis for every function:");
  std::ranges::for_each(
    analysis,
    [&](const PairFnRes& elem)
    {
      const auto& [function, metrics] = elem;
      std::println("  {}::{}{}: ",
                    function.filename,
                    (function.class_name.has_value() ?
                    function.class_name.value() + "::" :
                    ""),
                    function.name);

      std::ranges::for_each(
        metrics,
        [&](const MetricResult& result)
        {
          std::println("    {}: {}", result.metric_name, result.value);

          //
          // For std::variant
          //
          //std::visit(
          //  [](auto&& val)
          //  {
          //    std::println("{}", val);
          //  },
          //  result.value
          //);
        }
      );
    }
  );

  MetricsAccumulator accumulator;

  using namespace analyzer::metric_accumulator::metric_accumulator_impl;

  accumulator.RegisterAccumulator(CyclomaticComplexityMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());
  accumulator.RegisterAccumulator(NamingStyleMetric::kName,
                                  std::make_unique<CategoricalAccumulator>());
  accumulator.RegisterAccumulator(CodeLinesCountMetric::kName,
                                  std::make_unique<SumAverageAccumulator>());
  accumulator.RegisterAccumulator(CountParametersMetric::kName,
                                  std::make_unique<AverageAccumulator>());

  auto print_accumulated_analysis = [](const auto& accumulator)
  {
    auto& cc_acc_metric =
        accumulator.template GetFinalizedAccumulator<SumAverageAccumulator>(
          CyclomaticComplexityMetric::kName
        );

    std::println("    Sum Cyclomatic Complexity: {}",
                 cc_acc_metric.Get().sum);
    std::println("    Average Cyclomatic Complexity per function: {}",
                 cc_acc_metric.Get().average);

    auto& naming_acc_metric =
        accumulator.template GetFinalizedAccumulator<CategoricalAccumulator>(
          NamingStyleMetric::kName
        );

    std::ranges::for_each(
      naming_acc_metric.Get(),
      [](const auto& elem)
      {
        std::println("    Naming style '{}' is occured {} times",
                     elem.first, elem.second);
      }
    );

    auto& cl_acc_metric =
        accumulator.template GetFinalizedAccumulator<SumAverageAccumulator>(
          CodeLinesCountMetric::kName
        );

    std::println("    Sum Code lines count: {}", cl_acc_metric.Get().sum);
    std::println("    Average Code lines count per function: {}",
                 cl_acc_metric.Get().average);
    auto &cp_acc_metric =
        accumulator.template GetFinalizedAccumulator<AverageAccumulator>(
          CountParametersMetric::kName
        );
    std::println("    Average Parameters count per function: {}",
                 cp_acc_metric.Get());
  };

  auto analysis_by_files = analyzer::SplitByFiles(analysis);

  std::ranges::for_each(
    analysis_by_files,
    [&accumulator, &print_accumulated_analysis](const auto& analysis_view)
    {
      std::vector analysis_vec(analysis_view.begin(), analysis_view.end());

      analyzer::AccumulateFunctionAnalysis(analysis_vec, accumulator);
      std::println();
      std::println("Accumulated Analysis for file {}:",
                   analysis_vec.front().first.filename);
      print_accumulated_analysis(accumulator);
      accumulator.ResetAccumulators();
    }
  );

  auto analysis_by_classes = analyzer::SplitByClasses(analysis);

  std::ranges::for_each(
    analysis_by_classes,
    [&accumulator, &print_accumulated_analysis](const auto& analysis_view)
    {
      std::vector analysis_vec(analysis_view.begin(), analysis_view.end());

      analyzer::AccumulateFunctionAnalysis(analysis_vec, accumulator);

      std::println();
      std::println("Accumulated Analysis for сlass {}:",
                   analysis_vec.front().first.class_name.value());
      print_accumulated_analysis(accumulator);
      accumulator.ResetAccumulators();
  });

  analyzer::AccumulateFunctionAnalysis(analysis, accumulator);
  std::println();
  std::println("Accumulated Analysis for All Functions:");
  print_accumulated_analysis(accumulator);

  return 0;
}
