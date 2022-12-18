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
//      strategy_guide.push_back(GetMove(std::istringstream{line}));
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& move : strategy_guide) {
  //   std::cout << move.first << " " << move.second << "\n";
  // }

  // std::uint64_t result_p1 = 0;
  // std::uint64_t result_p2 = 0;

  {
    Timer t_main("calc");

    // for (const auto& move : strategy_guide) {
    //   result_p1 += PlayMatch(DecypherPart1(move));
    //   result_p1 += PlayMatch(DecypherPart2(move));
    // }
  }

  // std::cout << "\n";
  // std::cout << "part 1) total score: " << result_p1 << "\n";
  // std::cout << "part 2) total score: " << result_p2 << "\n";
  // std::cout << "\n";
}
