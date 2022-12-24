#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "timer.h"

enum class ParseState {
  MonkeyId,
  StartingItems,
  Operation,
  TestDivisibility,
  TrueAction,
  FalseAction
};

struct Monkey{
  std::vector<int> starting_items;
  char operation; // '+' or '*'
  std::optional<int> op_value;
  int test_div_by;
  int monkey_if_true;
  int monkey_if_false;
};

using Monkeys = std::vector<Monkey>;

int main()
{
  Timer t_main("main");

  Monkeys monkeys;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    ParseState state{};

    Monkey current_monkey{};

    std::string line;
    while (getline(infile, line)) {

      if (line.empty()) {
        continue;        
      }

      std::istringstream iss{line};

      bool valid_parse = false;
      switch(state) {
        case ParseState::MonkeyId: {
          std::string monkey_s{};
          int id;
          iss >> monkey_s >> id;

          if(monkey_s != "Monkey") {
            throw std::logic_error("parse expected monkey");
          }
          if(id != monkeys.size()) {
            throw std::logic_error("parse unexpected monkey id");
          }
          current_monkey = Monkey{};
          state = ParseState::StartingItems;
          break;
        }
        case ParseState::StartingItems: {
          std::string starting_s, items_s;
          int first_value;
          iss >> starting_s >> items_s >> first_value;
          assert(starting_s == "Starting" && items_s == "items:");
          current_monkey.starting_items.push_back(first_value);
          while(iss.rdbuf()->in_avail()) {
            char comma_c;
            int value;
            iss >> comma_c >> value;
            assert(comma_c == ',');
            current_monkey.starting_items.push_back(value);
          }
          state = ParseState::Operation;
          break;
        }
        case ParseState::Operation: {
          std::string op_s, new_s, eq_s, old_s, value_s;
          char op_c;
          iss >> op_s >> new_s >> eq_s >> old_s >> op_c >> value_s;

          if(op_s != "Operation:" || new_s != "new" || eq_s != "=" || old_s != "old") {
            throw std::logic_error("parse op invalid");
          }
          if(!(op_c == '*' || op_c == '+')) {
            throw std::logic_error("parse op operator");
          }
          current_monkey.operation = op_c;
          if(value_s != "old") {
            current_monkey.op_value = std::stoi(value_s);
          }
          state = ParseState::TestDivisibility;
          break;
        }
        case ParseState::TestDivisibility: {
          std::string test_s, divisible_s, by_s;
          iss >> test_s >> divisible_s >> by_s;
          assert( test_s == "Test:" && divisible_s == "divisible" && by_s == "by" );
          iss >> current_monkey.test_div_by;
          state = ParseState::TrueAction;
          break;
        }
        case ParseState::TrueAction: {
          std::string if_s, true_s, throw_s, to_s, monkey_s;
          iss >> if_s >> true_s >> throw_s >> to_s >> monkey_s;
          assert( if_s == "If" && true_s == "true:" && throw_s == "throw" &&
                  to_s == "to" && monkey_s == "monkey" );
          iss >> current_monkey.monkey_if_true;
          state = ParseState::FalseAction;
          break;
        }
        case ParseState::FalseAction: {
          std::string if_s, false_s, throw_s, to_s, monkey_s;
          iss >> if_s >> false_s >> throw_s >> to_s >> monkey_s;
          assert( if_s == "If" && false_s == "false:" && throw_s == "throw" &&
                  to_s == "to" && monkey_s == "monkey" );
          iss >> current_monkey.monkey_if_false;

          // final value, so save monkey now
          monkeys.push_back({});
          state = ParseState::MonkeyId;
          break;
        }
        default:
          throw std::logic_error("invalid parseing");
          break;
      }
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // input parse check
  for (std::size_t idx = 0; idx < monkeys.size(); idx++) {
    const auto& monkey = monkeys[idx];
    std::cout << "Monkey " << idx + 1 << ":\n";
    std::cout << "  Starting items: " << monkey.starting_items[0];
    for (auto item_itr = monkey.starting_items.begin() + 1;
         item_itr != monkey.starting_items.end();
         item_itr++) {
      std::cout << ", " << *item_itr;
    }
    std::cout << "  Operation: new = old ";
    if() << monkey.starting_items[0];
    
    std::cout << "\n";
  }
  std::cout << "\n";

  {
    Timer t_main("calc");

    // ...
  }

  // std::cout << "\n";
  // std::cout << "part 1) : " << result_p1 << "\n";
  // std::cout << "part 2) : " << result_p2 << "\n";
  // std::cout << "\n";
}
