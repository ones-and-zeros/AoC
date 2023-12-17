#include <cstdint>
#include <fstream>
#include <iostream>

#include "timer.h"

namespace {

void ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file\n");
  }

  std::string line;
  while (getline(infile, line)) {
    // std::istringstream iss{line};
    // std::string text;
    // int digits;
    // iss >> text >> digits;
  }

  infile.close();
}

std::int64_t CalcPart1(void) {
  Timer t_main("calc p1");

  std::int64_t value{};
  // TODO

  return value;
}

std::int64_t CalcPart2(void) {
  Timer t_main("calc p2");

  std::int64_t value{};
  // TODO

  return value;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath\n");
  }

  // Games games{ParseInput(argv[1])};

  // std::cout << games;

  auto result_p1 = CalcPart1();
  auto result_p2 = CalcPart2();

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
