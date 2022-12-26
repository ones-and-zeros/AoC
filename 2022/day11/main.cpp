#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "timer.h"

struct Monkey{
  std::vector<std::uint64_t> items;
  char operation; // '+' or '*'
  std::optional<int> op_value;
  int test_div_by;
  int monkey_if_true;
  int monkey_if_false;
};

using Monkeys = std::vector<Monkey>;

std::ostream& operator<<(std::ostream& os, const Monkey& monkey) {
  os << "  Starting items: " << monkey.items[0];
  for (auto item_itr = monkey.items.begin() + 1;
        item_itr != monkey.items.end();
        item_itr++) {
    os << ", " << *item_itr;
  }
  os << "\n";
  os << "  Operation: new = old " << monkey.operation << " ";
  if(monkey.op_value.has_value()) {
    os << monkey.op_value.value();
  } else {
    os << "old";
  }
  os << "\n";
  os << "  Test: divisible by " << monkey.test_div_by << "\n";
  os << "    If true: throw to monkey " << monkey.monkey_if_true << "\n";
  os << "    If false: throw to monkey " << monkey.monkey_if_false << "\n";
  os << "\n";

  return os;
}

std::ostream& operator<<(std::ostream& os, const Monkeys& monkeys) {
  for (std::size_t id = 0; id < monkeys.size(); id++) {
    const auto& monkey = monkeys[id];
    os << "Monkey " << id << ":\n";
    os << monkey;
  }
  return os;
}

enum class ParseState {
  MonkeyId,
  StartingItems,
  Operation,
  TestDivisibility,
  TrueAction,
  FalseAction
};

struct ParseUserState {
  ParseState state;
  Monkey current_monkey;
};

void ParseLine(const std::string& line,
               ParseUserState& user_state,
               Monkeys& monkeys) {
  if (line.empty()) {
    return;
  }
  ParseState& state{user_state.state};
  Monkey& current_monkey{user_state.current_monkey};

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
      current_monkey.items.push_back(first_value);
      while(iss.rdbuf()->in_avail()) {
        char comma_c;
        int value;
        iss >> comma_c >> value;
        assert(comma_c == ',');
        current_monkey.items.push_back(value);
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
      monkeys.push_back(current_monkey);
      state = ParseState::MonkeyId;
      break;
    }
    default:
      throw std::logic_error("invalid parseing");
      break;
  }
}

void PrintItems(const Monkeys& monkeys) {
  for (std::size_t id = 0; id < monkeys.size(); id++) {
    const auto& monkey = monkeys[id];
    std::cout << "Monkey " << id << ": ";
    const auto& items = monkey.items;
    for (auto it = items.begin(); it != items.end(); it++) {
      std::cout << *it;
      if (it + 1 != items.end()) {
        std::cout << ", ";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

using InspectedQtyPerMonkey = std::vector<size_t>;

void Round(Monkeys& monkeys, InspectedQtyPerMonkey& inspected_qty, bool do_div_3, std::uint32_t mod_by) {
  std::size_t id = 0;
  for (auto& monkey : monkeys) {
//    std::cout << "Monkey " << id << ":\n";
    inspected_qty[id++] += monkey.items.size();
    for(auto item : monkey.items) {
//      std::cout << " " << item << " -> ";
      auto operand = monkey.op_value == std::nullopt
                    ? item
                    : monkey.op_value.value(); 
      switch(monkey.operation) {
        case '*':
          item *= operand;
          break;
        case '+':
          item += operand;
          break;
        default:
          throw std::logic_error("unknown operation");
      }
//      std::cout << item << " -> ";
      if (do_div_3) {
        item /= 3;
      }

      item %= mod_by;

      bool div_by_pass = 0 == (item % monkey.test_div_by);
      auto rx_id = div_by_pass ? monkey.monkey_if_true : monkey.monkey_if_false;
      monkeys[rx_id].items.push_back(item);

//      std::cout << item << " to " << rx_id << "\n";
    }
    monkey.items.clear();
//    std::cout << "\n";
  }
}

std::uint64_t CalcResultPart1(const Monkeys& monkeys) {
//  PrintItems(monkeys);

  // use local copy for calculation
  auto monkeys_play = monkeys;

  std::uint32_t mod_by = 1;
  for(const auto& monkey : monkeys_play) {
    mod_by *= monkey.test_div_by;
  }

  InspectedQtyPerMonkey inspected_qty{};
  for (const auto& _ : monkeys) {
    inspected_qty.push_back({0});
  }
  for (std::size_t round = 0; round < 20; round++) {
    Round(monkeys_play, inspected_qty, true, mod_by);
//    PrintItems(monkeys_play);
  }

//  PrintItems(monkeys_play);

  // std::size_t id = 0;
  // for (const auto& qty : inspected_qty) {
  //   std::cout << "Monkey " << id++ << ": " << qty << "\n";
  // }

  std::sort(inspected_qty.begin(), inspected_qty.end(), std::greater<std::size_t>());
  
  return (std::uint64_t)inspected_qty[0] * inspected_qty[1];
}

std::uint64_t CalcResultPart2(const Monkeys& monkeys) {
//  PrintItems(monkeys);

  // use local copy for calculation
  auto monkeys_play = monkeys;

  std::uint32_t mod_by = 1;
  for(const auto& monkey : monkeys_play) {
    mod_by *= monkey.test_div_by;
  }

  InspectedQtyPerMonkey inspected_qty{};
  for (const auto& _ : monkeys) {
    inspected_qty.push_back({0});
  }
  for (std::size_t round = 0; round < 10000; round++) {
    Round(monkeys_play, inspected_qty, false, mod_by);

//     std::vector<std::size_t> rounds_to_report_after{1,20,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000};

//     if (rounds_to_report_after.end() != std::find(rounds_to_report_after.begin(),
//                                                   rounds_to_report_after.end(),
//                                                   round+1)) {
//       std::cout << "round " << round + 1 << "\n";

// //      PrintItems(monkeys_play);

//       std::size_t id = 0;
//       for (const auto& qty : inspected_qty) {
//         std::cout << "Monkey " << id++ << ": " << qty << "\n";
//       }
//       std::cout << "\n";
//     }
  }

  std::sort(inspected_qty.begin(), inspected_qty.end(), std::greater<std::size_t>());
  

  std::cout << inspected_qty[0] << " " << inspected_qty[1] << "\n";

  return (std::uint64_t)inspected_qty[0] * inspected_qty[1];
}

int main()
{
  Timer t_main("main");

  Monkeys monkeys;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    ParseUserState parse_user_state{};

    std::string line;
    while (getline(infile, line)) {
      ParseLine(line, parse_user_state, monkeys);
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // std::cout << monkeys;
  // std::cout << "\n";

  std::uint64_t result_p1{};
  std::uint64_t result_p2{};

  {
    Timer t_main("calc");

    result_p1 = CalcResultPart1(monkeys);
    result_p2 = CalcResultPart2(monkeys);
  }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
