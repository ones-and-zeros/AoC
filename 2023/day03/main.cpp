#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
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

bool operator<(const Coordinate &point_a, const Coordinate &point_b) {
  if (point_a.row < point_b.row) {
    return true;
  }
  if (point_a.row > point_b.row) {
    return false;
  }
  return (point_a.col == point_b.col);
}

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

struct Asset {
  int value;
  std::vector<Coordinate> points;
};

using Assets = std::set<Asset>;

bool operator<(const Asset &asset_a, const Asset &asset_b) {

  if (asset_a.value < asset_b.value) {
    return true;
  }
  if (asset_a.value > asset_b.value) {
    return false;
  }
  if (asset_a.points.size() < asset_b.points.size()) {
    return true;
  }
  if (asset_a.points.size() > asset_b.points.size()) {
    return false;
  }

  return (asset_a.points[0].col < asset_b.points[0].col);
}

std::optional<Asset> GetAsset(const Schematic &schematic,
                              const Coordinate &point) {

  if (!isdigit(schematic[point.row][point.col])) {
    return std::nullopt;
  }

  auto pos = point;
  while (true) {
    auto prior = Coordinate{pos.row, pos.col - 1};
    if (prior.col < 0) {
      break;
    }
    if (!isdigit(schematic[prior.row][prior.col])) {
      break;
    }
    pos = prior;
  }

  std::string text{};
  std::vector<Coordinate> points;
  while (isdigit(schematic[pos.row][pos.col])) {
    text += schematic[pos.row][pos.col];
    points.push_back(pos);
    pos = Coordinate{pos.row, pos.col + 1};
    if (pos.col >= schematic[0].size()) {
      break;
    }
  }

  return Asset{.value = std::stoi(text), .points = points};
}

std::int64_t CalcPart2(const Schematic &schematic) {
  Timer t_main("calc p2");

  std::int64_t value{};
  for (auto row = 0; row < schematic.size(); row++) {
    for (auto col = 0; col < schematic[0].size(); col++) {
      const auto current_char = schematic[row][col];
      const auto current_pos = Coordinate{row, col};

      if (current_char == '*') {
        Assets assets{};

        for (const auto &nearby : kNearby) {
          Coordinate pos{row + nearby.row, col + nearby.col};
          const auto maybe_asset = GetAsset(schematic, pos);
          if (maybe_asset.has_value()) {
            assets.insert(maybe_asset.value());
          }
        }

        if (assets.size() == 2) {
          const auto gear_ratio =
              (*assets.begin()).value * (*assets.rbegin()).value;
          value += gear_ratio;
        }
      }
    }
  }
  return value;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath");
  }

  auto schematic{ParseInput(argv[1])};

  // std::cout << games;

  auto result_p1 = CalcPart1(schematic);
  auto result_p2 = CalcPart2(schematic);

  std::cout << "\n";
  std::cout << "part 1: " << result_p1 << "\n";
  std::cout << "part 2: " << result_p2 << "\n";
  std::cout << "\n";
}
