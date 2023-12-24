#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "timer.h"

namespace {

using Pattern = std::vector<std::string>;
using Patterns = std::vector<Pattern>;

std::ostream &operator<<(std::ostream &os, const Pattern &pattern) {
  for (const auto &line : pattern) {
    os << line << "\n";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Patterns &patterns) {
  auto count = 0;
  for (const auto &pattern : patterns) {
    os << count++ << ":\n";
    os << pattern;
  }
  return os;
}

Patterns ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Patterns patterns{};
  Pattern pattern{};

  std::string line;
  while (getline(infile, line)) {
    if (line == "") {
      if (pattern.size()) {
        patterns.push_back(pattern);
        pattern.clear();
      }
      continue;
    }
    pattern.push_back(line);
  }
  if (pattern.size()) {
    patterns.push_back(pattern);
  }

  infile.close();

  return patterns;
}

bool IsMirrorRow(const Pattern &pattern, int row_offfest) {
  auto before = row_offfest - 1;
  int after = row_offfest;
  if (before < 0 || after >= pattern.size()) {
    return false;
  }
  while (before >= 0 && after < pattern.size()) {
    if (pattern[before] != pattern[after]) {
      return false;
    }

    before--;
    after++;
  }
  return true;
}

std::optional<int> FindMirrorRow(const Pattern &pattern) {
  for (int row = 1; row < pattern.size(); row++) {
    if (IsMirrorRow(pattern, row)) {
      return row;
    }
  }
  return std::nullopt;
}

constexpr bool WouldBeSeen(int total, int mirror, int maybe_included) {
  assert(maybe_included < total);

  auto mirrored_size = mirror * 2;
  if (mirrored_size < total) {
    return maybe_included < mirrored_size;
  }

  mirrored_size = (total - mirror) * 2;
  return maybe_included >= (total - mirrored_size);
};
static_assert(WouldBeSeen(5, 1, 0) == true);
static_assert(WouldBeSeen(5, 1, 1) == true);
static_assert(WouldBeSeen(5, 1, 2) == false);
static_assert(WouldBeSeen(5, 1, 3) == false);
static_assert(WouldBeSeen(5, 1, 4) == false);

static_assert(WouldBeSeen(5, 4, 0) == false);
static_assert(WouldBeSeen(5, 4, 1) == false);
static_assert(WouldBeSeen(5, 4, 2) == false);
static_assert(WouldBeSeen(5, 4, 3) == true);
static_assert(WouldBeSeen(5, 4, 4) == true);

static_assert(WouldBeSeen(7, 4, 0) == false);
static_assert(WouldBeSeen(7, 4, 1) == true);
static_assert(WouldBeSeen(7, 4, 2) == true);
static_assert(WouldBeSeen(7, 4, 3) == true);
static_assert(WouldBeSeen(7, 4, 4) == true);
static_assert(WouldBeSeen(7, 4, 5) == true);
static_assert(WouldBeSeen(7, 4, 6) == true);

std::optional<int> FindMirrorRowIncluding(const Pattern &pattern,
                                          int include_row) {
  const auto row_size = pattern.size();

  for (int row = 1; row < row_size; row++) {
    if (WouldBeSeen(row_size, row, include_row)) {
      if (IsMirrorRow(pattern, row)) {
        return row;
      }
    }
  }
  return std::nullopt;
}

bool IsMirrorCol(const Pattern &pattern, int col) {
  auto before = col - 1;
  int after = col;
  if (before < 0 || after >= pattern[0].size()) {
    return false;
  }
  while (before >= 0 && after < pattern[0].size()) {
    for (auto row = 0; row < pattern.size(); row++) {
      if (pattern[row][before] != pattern[row][after]) {
        return false;
      }
    }

    before--;
    after++;
  }
  return true;
}

std::optional<int> FindMirrorCol(const Pattern &pattern) {
  for (int col = 1; col < pattern[0].size(); col++) {
    if (IsMirrorCol(pattern, col)) {
      return col;
    }
  }
  return std::nullopt;
}

std::optional<int> FindMirrorColIncluding(const Pattern &pattern,
                                          int include_col) {
  const auto col_size = pattern[0].size();

  for (int col = 1; col < col_size; col++) {
    if (WouldBeSeen(col_size, col, include_col)) {
      if (IsMirrorCol(pattern, col)) {
        return col;
      }
    }
  }
  return std::nullopt;
}

std::optional<int> FindMirrorValue(const Pattern &pattern) {
  auto maybe_mirror_row = FindMirrorRow(pattern);
  if (maybe_mirror_row.has_value()) {
    // std::cout << "row: " << maybe_mirror_row.value() << "\n";
    return maybe_mirror_row.value() * 100;
  }
  auto maybe_mirror_col = FindMirrorCol(pattern);
  if (maybe_mirror_col.has_value()) {
    // std::cout << "col: " << maybe_mirror_col.value() << "\n";
    return maybe_mirror_col.value();
  }

  return std::nullopt;
}

std::int64_t CalcPart1(const Patterns &patterns) {
  Timer t_main("calc p1");

  std::int64_t total{0};
  for (const auto &pattern : patterns) {
    // std::cout << pattern;

    const auto maybe_value = FindMirrorValue(pattern);
    assert(maybe_value.has_value());
    total += maybe_value.value();

    // std::cout << "\n";
  }

  return total;
}

std::int64_t CalcPart2(const Patterns &patterns) {
  Timer t_main("calc p2");

  std::int64_t total{0};
  for (const auto &pattern : patterns) {
    const auto row_size = pattern.size();
    const auto col_size = pattern[0].size();
    const auto pixel_size = row_size * col_size;

    for (auto pixel = 0; pixel < pixel_size; pixel++) {
      const auto col = pixel % col_size;
      const auto row = pixel / col_size;

      // attempted smudge fix
      auto local_pattern = pattern;
      local_pattern[row][col] = local_pattern[row][col] == '.' ? '#' : '.';

      auto maybe_mirror_row = FindMirrorRowIncluding(local_pattern, row);
      if (maybe_mirror_row.has_value()) {
        // std::cout << "row: " << maybe_mirror_row.value() << "\n";
        total += maybe_mirror_row.value() * 100;
        break;
      }
      auto maybe_mirror_col = FindMirrorColIncluding(local_pattern, col);
      if (maybe_mirror_col.has_value()) {
        // std::cout << "col: " << maybe_mirror_col.value() << "\n";
        total += maybe_mirror_col.value();
        break;
      }

      // revert attempted smudge fix
      local_pattern[row][col] = local_pattern[row][col] == '.' ? '#' : '.';
    }

    // std::cout << "\n";
  }

  return total;

  return 0;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath");
  }

  auto input = ParseInput(argv[1]);

  auto result_p1 = CalcPart1(input);
  auto result_p2 = CalcPart2(input);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
