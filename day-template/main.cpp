#include <cstdint>
#include <fstream>
#include <iostream>

#include "timer.h"

int main()
{
  Timer t_main("main");

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      // ...
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (...) {
  //   std::cout << ... << "\n";
  // }

  {
    Timer t_main("calc");

    // ...
  }

  // std::cout << "\n";
  // std::cout << "part 1) : " << result_p1 << "\n";
  // std::cout << "part 2) : " << result_p2 << "\n";
  // std::cout << "\n";
}
