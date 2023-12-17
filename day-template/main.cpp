#include <cstdint>
#include <fstream>
#include <iostream>

#include "timer.h"

std::int64_t CalcPart1() {
  Timer t_main("calc p1");
  return 0;
}

std::int64_t CalcPart2() {
  Timer t_main("calc p2");
  return 0;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "error, expected 1 argument for input filepath\n";
    return 1;
  }

  std::ifstream infile{argv[1]};
  if (!infile.is_open()) {
    std::cout << "error, unable to open input file\n";
    return 2;
  }

  {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      // ...
    }

    infile.close();
  }
  std::cout << "\n";

  // // input parse check
  // for (...) {
  //   std::cout << ... << "\n";
  // }

  auto result_p1 = CalcPart1();
  auto result_p2 = CalcPart2();

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
