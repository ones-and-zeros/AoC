#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "timer.h"

namespace {

using Dish = std::vector<std::string>;

std::ostream &operator<<(std::ostream &os, const Dish &dish) {
  for (const auto &row : dish) {
    os << row << "\n";
  }
  return os;
}

Dish ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Dish dish{};

  std::string line;
  while (getline(infile, line)) {
    dish.push_back(line);
  }

  infile.close();

  return dish;
}

constexpr auto kCharRockRound{'O'};
constexpr auto kCharRockCube{'#'};
constexpr auto kCharEmpty{'.'};

Dish TiltNorth(const Dish &dish_original) {
  const auto row_size = dish_original.size();
  const auto col_size = dish_original[0].size();

  auto dish = dish_original;

  for (auto col = 0U; col < col_size; col++) {
    auto drop_row = 0U;
    for (auto read_row = 0U; read_row < row_size; read_row++) {
      if (dish[read_row][col] == kCharRockCube) {
        drop_row = read_row + 1;
        continue;
      }
      while (drop_row < read_row) {
        if (dish[drop_row][col] == kCharEmpty) {
          break;
        }
        drop_row++;
      }
      if (drop_row < read_row) {
        if (dish[read_row][col] == kCharRockRound) {
          dish[read_row][col] = kCharEmpty;
          dish[drop_row][col] = kCharRockRound;
          drop_row++;
        }
      }
    }
  }

  return dish;
}

std::int64_t CalcPart1(const Dish &dish) {
  Timer t_main("calc p1");

  // std::cout << dish << "\n";

  const auto tilted_dish = TiltNorth(dish);

  // std::cout << tilted_dish << "\n";

  std::int64_t total{0};

  for (auto row = 0U; row < tilted_dish.size(); row++) {
    for (auto col = 0U; col < tilted_dish[0].size(); col++) {
      if (tilted_dish[row][col] == kCharRockRound) {
        total += (tilted_dish.size() - row);

        // std::cout << "score " << (tilted_dish.size() - row) << "\n";
      }
    }
  }

  return total;
}

std::int64_t CalcPart2(const Dish &dish) {
  Timer t_main("calc p2");

  return 0;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath");
  }

  auto input = ParseInput(argv[1]);

  auto result_p1 = CalcPart1(input);
  auto result_p2 = CalcPart2(input);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
