#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

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

std::int64_t CalcPart1(const Input &input) {
  Timer t_main("calc p1");

  std::int64_t count{};

  std::string location = kStart;
  while (location != kEnd) {
    const auto &match = input.lookup.at(location);
    location = input.instructions[count % input.instructions.size()] == 'L'
                   ? match.left
                   : match.right;
    count++;
    // std::cout << location << "\n";
  }

  return count;
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

  // std::cout << games;

  auto result_p1 = CalcPart1(input);
  auto result_p2 = CalcPart2(input);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
