#pragma once
#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"

namespace fs = std::filesystem;
namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyzer::function {

struct Function
{
  std::string filename;
  std::optional<std::string> class_name;
  std::string name;
  std::string ast;

  std::string ToString() const
  {
    static const std::string line(80, '=');
    return std::vformat(
      "{}\n< filename = '{}', class_name = '{}', name = '{}'>\nAST:\n{}\n{}\n",
      std::make_format_args(
        line,
        filename,
        // This is so fucking bullshit, you can't even write
        // class_name.value_or("shite"), which would be logical here.
        (class_name ? *class_name : "(none)"),
        name,
        ast,
        line
      )
    );
  }
};

struct FunctionExtractor
{
    std::vector<Function> Get(const analyzer::file::File& file);

  private:
    struct Position
    {
      size_t line;
      size_t col;
    };

    struct FunctionNameLocation
    {
      Position start;
      Position end;
      std::string name;
    };

    struct ClassInfo
    {
      std::string name;
      Position start;
      Position end;
    };

    FunctionNameLocation GetNameLocation(const std::string &function_ast);

    std::string GetNameFromSource(const std::string &function_ast,
                                  const std::vector<std::string> &lines);

    std::optional<ClassInfo>
    FindEnclosingClass(const std::string &ast,
                       const FunctionNameLocation &func_loc);

    std::string GetClassNameFromSource(const ClassInfo &class_info,
                                       const std::vector<std::string> &lines);
};

}  // namespace analyzer::function
