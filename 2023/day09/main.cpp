#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "timer.h"

namespace {

using Sequence = std::vector<std::int64_t>;

using Input = std::vector<Sequence>;

Input ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Input input{};

  std::string line;
  while (getline(infile, line)) {
    Sequence sequence{};
    std::istringstream iss{line};
    int digits;
    while (iss >> digits) {
      sequence.push_back(digits);
    }
    input.push_back(sequence);
  }

  infile.close();

  return input;
}

bool AreAllZero(const Sequence &sequence) noexcept {
  for (const auto &value : sequence) {
    if (value != 0) {
      return false;
    }
  }
  return true;
}

std::int64_t CalcPart1(const Input &input) {
  Timer t_main("calc p1");

  std::vector<std::int64_t> results{};

  for (const auto &sequence : input) {
    std::vector<Sequence> patterns{};
    patterns.push_back(sequence);

    while (!AreAllZero(patterns[patterns.size() - 1])) {
      Sequence derived_pattern{};
      const auto &last_pattern = patterns[patterns.size() - 1];
      for (auto pos = 1; pos < last_pattern.size(); pos++) {
        derived_pattern.push_back(last_pattern[pos] - last_pattern[pos - 1]);
      }
      patterns.push_back(derived_pattern);
    }

    // for (const auto &pattern : patterns) {
    //   for (const auto &value : pattern) {
    //     std::cout << " " << value;
    //   }
    //   std::cout << "\n";
    // }
    // std::cout << "\n";

    for (auto pos = 0; pos < patterns.size() - 1; pos++) {
      auto &to_modify = patterns[patterns.size() - (pos + 2)];
      const auto &to_reference = patterns[patterns.size() - (1 + pos)];

      // std::cout << " " << *to_modify.rbegin() << " " <<
      // *to_reference.rbegin() << "\n";

      to_modify.push_back(*to_modify.rbegin() + *to_reference.rbegin());
    }

    // for (const auto &pattern : patterns) {
    //   for (const auto &value : pattern) {
    //     std::cout << " " << value;
    //   }
    //   std::cout << "\n";
    // }
    // std::cout << "\n";

    results.push_back(*patterns[0].rbegin());
  }

  return std::accumulate(results.begin(), results.end(), 0);
}

std::int64_t CalcPart2(const Input &input) {
  Timer t_main("calc p2");

  std::int64_t value{};
  // TODO

  return value;
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
}
