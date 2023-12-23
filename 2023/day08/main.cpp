#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "timer.h"

namespace {

struct Match {
  std::string left;
  std::string right;
};

using Lookup = std::map<std::string, Match>;

struct Input {
  std::string instructions;
  Lookup lookup;
};

Input ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Input input;
  getline(infile, input.instructions);

  std::string line;
  while (getline(infile, line)) {
    std::istringstream iss{line};
    if (line.size() == 0) {
      continue;
    }
    std::string key, eq, left, right;

    iss >> key >> eq >> left >> right;
    left.erase(left.begin());
    left.erase(left.end() - 1);
    right.erase(right.end() - 1);

    // std::cout << key << ": "  << left << " " << right << "\n";
    input.lookup.insert({key, Match{left, right}});
  }

  infile.close();

  return input;
}

constexpr auto kStart = "AAA";
constexpr auto kEnd = "ZZZ";

std::string Next(const Input &input, const std::string &current,
                 std::int64_t offset) noexcept {
  return (input.instructions[offset % input.instructions.size()] == 'L')
             ? input.lookup.at(current).left
             : input.lookup.at(current).right;
}

std::int64_t CalcPart1(const Input &input) {
  Timer t_main("calc p1");

  std::int64_t count{};

  // std::string location = kStart;
  // while (location != kEnd) {
  //   const auto &match = input.lookup.at(location);
  //   location = input.instructions[count % input.instructions.size()] == 'L'
  //                  ? match.left
  //                  : match.right;
  //   count++;
  //   // std::cout << location << "\n";
  // }

  return count;
}

struct OffsetAndRate {
  std::int64_t offset;
  std::int64_t rate;
};

OffsetAndRate FindOffsetAndRate(const Input &input,
                                const std::string &start) noexcept {
  std::string current = start;
  std::int64_t offset = 0;
  for (; current[2] != 'Z'; offset++) {
    current = Next(input, current, offset);
  }

  const std::int64_t match_offset = offset;
  std::string end_found = current;

  current = Next(input, current, offset++);

  for (; current[2] != 'Z'; offset++) {
    current = Next(input, current, offset);
  }
  assert(end_found == current);
  const std::int64_t rate = offset - match_offset;

  current = Next(input, current, offset++);

  for (; current[2] != 'Z'; offset++) {
    current = Next(input, current, offset);
  }
  assert(end_found == current);
  assert(rate * 2 == offset - match_offset);

  return OffsetAndRate{match_offset, rate};
}

OffsetAndRate FindCommon(const OffsetAndRate &offset_rate_a,
                         const OffsetAndRate &offset_rate_b) noexcept {
  std::int64_t current_a = offset_rate_a.offset;
  std::int64_t current_b = offset_rate_b.offset;

  while (current_a != current_b) {
    if (current_a < current_b) {
      current_a += offset_rate_a.rate;
    } else {
      current_b += offset_rate_b.rate;
    }
  }

  return OffsetAndRate{
      .offset = current_a,
      .rate = std::lcm(offset_rate_a.offset, offset_rate_b.offset)};
}

std::int64_t CalcPart2(const Input &input) {
  Timer t_main("calc p2");

  std::vector<std::string> locations{};
  std::vector<OffsetAndRate> offsets_and_rates{};
  for (const auto &mapping : input.lookup) {
    if (mapping.first[2] == 'A') {
      // locations.push_back(mapping.first);
      offsets_and_rates.push_back(FindOffsetAndRate(input, mapping.first));
    }
  }

  // for (auto pos = 0U; pos < locations.size(); pos++) {
  //   std::cout << " " << locations[pos];
  //   std::cout << " {" << offsets_and_rates[pos].offset;
  //   std::cout << ", " << offsets_and_rates[pos].rate << "}\n";
  // }

  for (auto pos = 0U; pos < offsets_and_rates.size() - 1; pos++) {
    offsets_and_rates[pos + 1] =
        FindCommon(offsets_and_rates[pos], offsets_and_rates[pos + 1]);
  }

  return offsets_and_rates[offsets_and_rates.size() - 1].offset;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath");
  }

  auto input = ParseInput(argv[1]);

  // std::cout << games;

  auto result_p1 = CalcPart1(input);
  auto result_p2 = CalcPart2(input);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
