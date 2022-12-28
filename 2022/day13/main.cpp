#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <vector>
#include <utility>

#include "timer.h"


enum class ParseState {
  First,
  Second,
};

enum class Type {
  Integer,
  List
};

struct Item {
  Type type;
  int integer;
  std::vector<Item> list;
};

using Packet = std::pair<Item, Item>;
using Packets = std::vector<Packet>;

std::ostream& operator<<(std::ostream& os, const Item& item) {
  switch (item.type) {
    case Type::Integer:
      os << item.integer;
      break;
    case Type::List: {

      os << '[';     
      auto size = item.list.size();
      for(std::size_t pos = 0; pos < size; pos++) {
        os << item.list[pos];
        if ( (pos + 1) != size) {
          os << ",";
        }
      }
      os << ']';
      break;
    }
    default:
      std::cout << std::endl;
      throw std::logic_error("op<< unexpected state");
  }
  return os;
}

auto FindClosingBracket(const std::string::const_iterator begin, const std::string::const_iterator end) {
  std::size_t depth{};

  auto it = begin;
  for ( ; it<end; it++) {
    switch (*it) {
      case '[':
        depth++;
        break;
      case ']':
        if (depth == 0) {
          return it;
        }
        depth--;
        break;
      default:
        break;
    }
  }

  std::cout << std::endl;
  throw std::logic_error("no closing bracket");
}

std::pair<Item,std::string::const_iterator> GetItem(
          std::string::const_iterator begin,
          std::string::const_iterator end) {

  if (isdigit(*begin)) {
    std::istringstream iss{{begin, end}};
    int value;
    iss >> value;
    return {Item{.type=Type::Integer, .integer=value}, end - iss.rdbuf()->in_avail()};
  }

  if (*begin == '[') {
    Item item{.type=Type::List};
    const auto closing = FindClosingBracket(begin + 1, end);

    for (auto it = begin + 1; it < closing; ) {
      auto [new_item, new_it] = GetItem(it, closing);
      item.list.push_back(new_item);
      it = new_it;

      if (*it == ',') {
        it++;
      }
    }

    return {item, closing+1};
  }

  std::string error_msg;
  error_msg += "unexpected lead char \'";
  error_msg += *begin;
  error_msg += "\'";

  std::cout << std::endl;
  throw std::logic_error(error_msg);
}

Item ParseItem(const std::string& line) {
  const auto [item, end] = GetItem(line.begin(), line.end());
  if(end != line.end()) {
    std::cout << std::endl;
    throw std::logic_error("incomplete item");
  }

  return item;
}

enum class OrderStatus {
  Pending,
  Correct,
  Incorrect
};

OrderStatus IsCorrectOrder(const Item& left, const Item& right) {
//  std::cout << left << " vs " << right;

  if (left.type == Type::Integer &&
      right.type == Type::Integer ) {
    if(left.integer == right.integer) {
//      std::cout << " same\n";
      return OrderStatus::Pending;
    }
    if(left.integer > right.integer) {
//      std::cout << " incorrect\n";
      return OrderStatus::Incorrect;
    }
//    std::cout << " correct\n";
    return OrderStatus::Correct;
  }

  if (left.type == Type::List &&
      right.type == Type::List ) {
    std::size_t qty = std::min(left.list.size(), right.list.size());

    for (std::size_t pos = 0; pos < qty; pos++) {
//      std::cout << "\n";
      auto result = IsCorrectOrder(left.list[pos], right.list[pos]);
      if (result == OrderStatus::Pending) {
        continue;
      }
      if (result == OrderStatus::Incorrect) {
//        std::cout << " incorrect\n";
        return OrderStatus::Incorrect;
      }
//      std::cout << " correct\n";
      return OrderStatus::Correct;
    }

    if (left.list.size() < right.list.size()) {
//      std::cout << " correct\n";
      return OrderStatus::Correct;
    }
    if (left.list.size() > right.list.size()) {
//      std::cout << " incorrect\n";
      return OrderStatus::Incorrect;
    }
//    std::cout << " same\n";
    return OrderStatus::Pending;
  }

  // only one is integer, other is list
  if (left.type == Type::Integer) {
    Item promoted_list{.type=Type::List};
    promoted_list.list.push_back(Item{.type=Type::Integer, left.integer});
//    std::cout << "\n";
    return IsCorrectOrder(promoted_list, right);
  }
  if (right.type == Type::Integer) {
    Item promoted_list{.type=Type::List};
    promoted_list.list.push_back(Item{.type=Type::Integer, right.integer});
//    std::cout << "\n";
    return IsCorrectOrder(left, promoted_list);
  }

//  std::cout << std::endl;
  throw std::logic_error("unexpected check");  
}

std::size_t CalcPart1(const Packets& packets) {
  std::size_t total{0};
  for (std::size_t pos = 0; pos < packets.size(); ++pos) {
    const auto result = IsCorrectOrder(packets[pos].first, packets[pos].second);
    if (result == OrderStatus::Incorrect) {
      continue;
    }
    if (result == OrderStatus::Correct) {
      total += pos + 1;
      continue;
    }
    std::cout << std::endl;
    throw std::logic_error("undecided!");  
  }

  return total;
}

bool operator<(const Item& a, const Item& b) {
  auto result = IsCorrectOrder(a,b);
  return result == OrderStatus::Correct;
}

bool operator==(const Item& a, const Item& b) {
  auto result = IsCorrectOrder(a,b);
  return result == OrderStatus::Pending;
}

std::vector<Item> GetKeys() {
  const std::vector<std::string> keys_s{{"[[2]]"},{"[[6]]"}};

  std::vector<Item> keys;
  for (const auto& key_s : keys_s) {
    keys.push_back(ParseItem(key_s));
  }
  return keys;
}

std::size_t CalcPart2(const Packets& packets) {
  std::vector<Item> items;

  for (std::size_t pos = 0; pos < packets.size(); ++pos) {
    items.push_back({packets[pos].first});
    items.push_back({packets[pos].second});
  };

  auto keys{GetKeys()};

  for (const auto& key : keys) {
    items.push_back(key);
  }

  std::sort(items.begin(), items.end(),
      [](const Item& a, const Item& b) {
        auto result = IsCorrectOrder(a,b);
        return result != OrderStatus::Incorrect;
      });

  std::size_t total{1};
  for (const auto& key : keys) {
    const auto found = std::find(items.begin(), items.end(), key);
    if(found == items.end()) {
      std::cout << std::endl;
      throw std::logic_error("missing key");  
    }
    total *= (found - items.begin()) + 1;
  }

  // for (const auto& item : items) {
  //   std::cout << item << "\n";
  // }

  return total;
}

int main()
{
  Timer t_main("main");

  Packets packets;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    ParseState parse_state{};

    std::string line;
    Item left_item;
    while (getline(infile, line)) {
      switch(parse_state) {
        case ParseState::First:
          if (!line.empty()) {
            left_item = ParseItem(line);
            parse_state = ParseState::Second;
          }
          break;
        case ParseState::Second:
          if (line.empty()) {
            std::cout << std::endl;
            throw std::logic_error("unexpected empty line");
          }
          packets.push_back({left_item,ParseItem(line)});
          parse_state = ParseState::First;
          break;
        default:
          std::cout << std::endl;
          throw std::logic_error("parse unexpected state");
      }
    }

    infile.close();
  }
  else {
    std::cout << std::endl;
    throw std::logic_error("unable to open input file");
  }
  std::cout << "\n";

  // // input parse check
  // for (const auto& packet : packets) {
  //   std::cout << packet.first << "\n";
  //   std::cout << packet.second << "\n";
  //   std::cout << "\n";
  // }

  std::size_t result_p1{};
  std::size_t result_p2{};

  {
    Timer t_main("calc");

    result_p1 = CalcPart1(packets);
    result_p2 = CalcPart2(packets);
  }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
