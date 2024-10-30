#include <cstdint>
#include <fstream>
#include <iostream>

#include "timer.h"

namespace {

using Input = int;

Input ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  std::string line;
  while (getline(infile, line)) {
    // std::istringstream iss{line};
    // std::string text;
    // int digits;
    // iss >> text >> digits;
  }

  infile.close();

  return 0;
}

std::int64_t CalcPart1(const Input &input) {
  Timer t_main("calc p1");

  return 0;
}

std::int64_t CalcPart2(const Input &input) {
  Timer t_main("calc p2");

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
