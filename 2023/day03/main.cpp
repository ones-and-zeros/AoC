#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

namespace {

using Schematic = std::vector<std::string>;

Schematic ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Schematic schematic{};
  std::string line;
  while (getline(infile, line)) {
    if ((schematic.size() > 0) && (schematic[0].size() != line.size())) {
      throw std::logic_error("wrong width");
    }
    schematic.push_back(line);
  }

  infile.close();

  return schematic;
}

bool IsSymbol(char ch) {
  if (std::isdigit(ch) || ch == '.') {
    return false;
  }
  return true;
}

struct Coordinate {
  int row;
  int col;
};

constexpr std::array<Coordinate, 9> kNearby{
    Coordinate{-1, -1}, Coordinate{-1, 0}, Coordinate{-1, 1}, Coordinate{0, -1},
    Coordinate{0, 1},   Coordinate{1, -1}, Coordinate{1, 0},  Coordinate{1, 1},
};

std::int64_t CalcPart1(const Schematic &schematic) {
  Timer t_main("calc p1");

  std::int64_t value{};
  std::string current_digits{};
  bool valid = false;
  for (auto row = 0; row < schematic.size(); row++) {
    if (valid) {
      value += std::stoi(current_digits);
    }
    valid = false;
    current_digits.clear();

    for (auto col = 0; col < schematic[0].size(); col++) {
      const auto current_char = schematic[row][col];
      const auto current_pos = Coordinate{row, col};

      if (!std::isdigit(current_char)) {
        if (valid) {
          value += std::stoi(current_digits);
        }
        valid = false;
        current_digits.clear();
        continue;
      }
      current_digits += current_char;
      if (valid) {
        continue;
      }
      for (const auto &nearby : kNearby) {
        Coordinate pos{row + nearby.row, col + nearby.col};
        if (pos.row < 0 || pos.row >= schematic.size()) {
          continue;
        }
        if (pos.col < 0 || pos.col >= schematic[0].size()) {
          continue;
        }
        if (IsSymbol(schematic[pos.row][pos.col])) {
          valid = true;
          break;
        }
      }
    }
  }

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

  auto schematic{ParseInput(argv[1])};

  // std::cout << games;

  auto result_p1 = CalcPart1(schematic);
  auto result_p2 = CalcPart2();

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
