#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

using Crate = char;
using Stack = std::vector<Crate>;
using Stacks = std::vector<Stack>;

enum class InputParse {
  Setup,
  Stacking,
  Procedure,
};

struct Step {
  int quantity;
  int from;
  int to;
};

using Procedure = std::vector<Step>;

Step ParseProcedureLine(const std::string& line) {
  if (line.empty()) {
    throw std::logic_error("expected upper");
  }

  std::istringstream iss{line};
  std::string move_s, from_s, to_s;
  int move, from, to;

  iss >> move_s >> move >> from_s >> from >> to_s >> to;

  if ( (move_s != "move") ||
       (from_s != "from") ||
       (to_s != "to") ) {
    throw std::logic_error("expected upper");
  }

  return Step{move, from - 1, to - 1};
}

std::string ResultPart1(const Procedure& procedure, Stacks stacks) {
  std::string result;
  for (const auto& step : procedure ) {
    for (std::size_t count = 0; count < step.quantity; count++) {
      stacks[step.to].push_back( stacks[step.from].back() );
      stacks[step.from].pop_back();
    }
  }
  for (const auto& stack : stacks) {
    result.push_back(stack.back());
  }
  return result;
}

std::string ResultPart2(const Procedure& procedure, Stacks stacks) {
  std::string result;
  for (const auto& step : procedure ) {
    
    stacks[step.to].insert( stacks[step.to].end(), 
                            stacks[step.from].end() - step.quantity,
                            stacks[step.from].end());

    stacks[step.from].erase( stacks[step.from].end() - step.quantity,
                             stacks[step.from].end());
  }
  for (const auto& stack : stacks) {
    result.push_back( stack.back() );
  }
  return result;
}

int main()
{
  Timer t_main("main");

  Stacks stacks;
  Procedure procedure;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    InputParse parse_state{InputParse::Setup};

    std::string line;
    while (getline(infile, line)) {
      switch (parse_state) {
        case InputParse::Setup: {
          stacks.resize((line.size() + 1) / 4);
          parse_state = InputParse::Stacking;
          // intentional fallthrough
        }
        case InputParse::Stacking: {
          if (line.empty()) {
            parse_state = InputParse::Procedure;
            break;
          }
          if (std::isdigit(line[1])) break;

          std::size_t stack_id = 0;
          for(std::size_t char_idx = 1; char_idx < line.size(); char_idx += 4) {
            const char c = line[char_idx];
            if (std::isupper(c)) {
              stacks[stack_id].insert(stacks[stack_id].begin(), c);
            } else if (' ' != c) {
              throw std::logic_error("expected upper");
            }
            stack_id++;
          }
          break;
        }
        case InputParse::Procedure:
          procedure.push_back( ParseProcedureLine(line) );
          break;
      }
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // check input parsing
  // for (std::size_t stack_id = 0; stack_id < stacks.size(); stack_id++) {
  //   std::cout << stack_id + 1 << ": ";
  //   for (const auto& crate : stacks[stack_id]) {
  //     std:: cout << crate;
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "\n";
  // for (const auto& step : procedure) {
  //   std::cout << step.quantity << " : "
  //             << step.from << " -> "
  //             << step.to << "\n";
  // }
  // std::cout << "\n";

  std::string result_p1;
  std::string result_p2;

  {
    Timer t_main("calc");

    result_p1 = ResultPart1(procedure, stacks);
    result_p2 = ResultPart2(procedure, stacks);
 }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
