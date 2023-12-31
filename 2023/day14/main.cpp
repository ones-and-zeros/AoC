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

Dish TiltNorth(Dish &dish) {
  const auto row_size = dish.size();
  const auto col_size = dish[0].size();

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

Dish TiltSouth(Dish &dish) {
  const auto row_size = dish.size();
  const auto col_size = dish[0].size();

  for (auto col = 0U; col < col_size; col++) {
    int drop_row = row_size - 1;
    for (int read_row = row_size - 1; read_row >= 0; read_row--) {
      if (dish[read_row][col] == kCharRockCube) {
        drop_row = read_row - 1;
        continue;
      }
      while (drop_row > read_row) {
        if (dish[drop_row][col] == kCharEmpty) {
          break;
        }
        drop_row--;
      }
      if (drop_row > read_row) {
        if (dish[read_row][col] == kCharRockRound) {
          dish[read_row][col] = kCharEmpty;
          dish[drop_row][col] = kCharRockRound;
          drop_row--;
        }
      }
    }
  }

  return dish;
}

Dish TiltEast(Dish &dish) {
  const auto row_size = dish.size();
  const auto col_size = dish[0].size();

  for (auto row = 0U; row < row_size; row++) {
    int drop_col = row_size - 1;
    for (int read_col = row_size - 1; read_col >= 0; read_col--) {
      if (dish[row][read_col] == kCharRockCube) {
        drop_col = read_col - 1;
        continue;
      }
      while (drop_col > read_col) {
        if (dish[row][drop_col] == kCharEmpty) {
          break;
        }
        drop_col--;
      }
      if (drop_col > read_col) {
        if (dish[row][read_col] == kCharRockRound) {
          dish[row][read_col] = kCharEmpty;
          dish[row][drop_col] = kCharRockRound;
          drop_col--;
        }
      }
    }
  }

  return dish;
}

Dish TiltWest(Dish &dish) {
  const auto row_size = dish.size();
  const auto col_size = dish[0].size();

  for (auto row = 0U; row < row_size; row++) {
    auto drop_col = 0U;
    for (auto read_col = 0U; read_col < row_size; read_col++) {
      if (dish[row][read_col] == kCharRockCube) {
        drop_col = read_col + 1;
        continue;
      }
      while (drop_col < read_col) {
        if (dish[row][drop_col] == kCharEmpty) {
          break;
        }
        drop_col++;
      }
      if (drop_col < read_col) {
        if (dish[row][read_col] == kCharRockRound) {
          dish[row][read_col] = kCharEmpty;
          dish[row][drop_col] = kCharRockRound;
          drop_col++;
        }
      }
    }
  }

  return dish;
}

std::int64_t CalculateLoad(const Dish &dish) {
  std::int64_t total{0};

  for (auto row = 0U; row < dish.size(); row++) {
    for (auto col = 0U; col < dish[0].size(); col++) {
      if (dish[row][col] == kCharRockRound) {
        total += (dish.size() - row);
      }
    }
  }

  return total;
}

std::int64_t CalcPart1(const Dish &dish) {
  Timer t_main("calc p1");

  auto tilted_dish = dish;
  TiltNorth(tilted_dish);

  return CalculateLoad(tilted_dish);
}

void RotateCycles(Dish &dish, const std::int64_t cycles_total) {
  for (std::int64_t count = 0; count < cycles_total; count++) {
    TiltNorth(dish);
    TiltWest(dish);
    TiltSouth(dish);
    TiltEast(dish);
  }
}

std::int64_t CalcPart2(const Dish &dish) {
  Timer t_main("calc p2");

  constexpr auto kTotalCycles = 1'000'000'000;
  constexpr auto kFirstBatch = 1'000;

  auto tilted_dish = dish;
  RotateCycles(tilted_dish, kFirstBatch);

  auto dish_after_first_batch = tilted_dish;

  for (std::int64_t count = kFirstBatch; count < kTotalCycles; count++) {
    RotateCycles(tilted_dish, 1);
    if (dish_after_first_batch == tilted_dish) {
      const std::int64_t rate = count + 1 - kFirstBatch;
      // std::cout << "dup at " << count << "\n";
      count += ((kTotalCycles - count) / rate) * rate;
      // std::cout << "offset to " << count << "\n";
    }
  }

  return CalculateLoad(tilted_dish);
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
