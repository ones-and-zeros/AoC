#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

#include "timer.h"


enum class Opcode {
  Noop,
  Addx
};

std::ostream& operator<<(std::ostream& os, const Opcode opcode) {
  switch (opcode) {
    case Opcode::Noop:
      os << "noop";
      break;
    case Opcode::Addx:
      os << "noop";
      break;
    default:
      os << "(?)";
      break;
  }

  return os;
}

struct Cmd {
  Opcode opcode;
  int value;
};

using Cmds = std::vector<Cmd>;

std::pair<int,std::string> CalParts(const Cmds& cmds) {

  std::vector<int> results;

  int x = 1;

  for (const auto& cmd : cmds) {
    // all commands take at least one cycle, and the first cycle has no effect
    results.push_back(x);

    if(cmd.opcode == Opcode::Addx) {
      results.push_back(x);
      x += cmd.value;
    }
  }

  int result_p1{};
  std::vector<int> points{{20,60,100,140,180,220}};  
  for(const auto point : points) {
    result_p1 += results[point - 1] * point;
  }

  std::string result_p2{};
  for(int cycle = 0; cycle < 240; cycle++) {
    if(std::abs((cycle % 40) - results[cycle]) <= 1 ) {
      result_p2.push_back('#');
      continue;
    }
    result_p2.push_back('.');
  }

  return {result_p1, result_p2};
}


int main()
{
  Timer t_main("main");

  Cmds commands;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      std::istringstream iss{line};
      std::string opcode{};

      iss >> opcode;

      if(opcode == "noop") {
        commands.push_back({Opcode::Noop, 0});
        continue;
      }
      
      if(opcode == "addx") {
        int val;
        iss >> val;
        commands.push_back({Opcode::Addx, val});
        continue;
      }

      throw std::logic_error("unxpected opcode");
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& cmd : commands) {
  //   std::cout << cmd.opcode << " " << cmd.value << "\n";
  // }
  // std::cout << "\n";

  int result_p1{};
  std::string result_p2{};

  {
    Timer t_main("calc");

    const auto& [p1, p2] = CalParts(commands);
    result_p1 = p1;
    result_p2 = p2;
  }

  std::cout << "\n";
  std::cout << "part 1) " << result_p1 << "\n";
  std::cout << "part 2) \n";

  constexpr std::size_t display_w{40};
  constexpr std::size_t display_h{6};
  for(int y = 0; y < display_h; y++) {
    std::cout << result_p2.substr(y*display_w, display_w) << "\n";
  }
  
  std::cout << "\n";
}
