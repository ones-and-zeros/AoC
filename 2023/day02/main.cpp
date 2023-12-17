#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

namespace {

struct Cubes {
  int red;
  int green;
  int blue;
};

const std::array<std::string, 3> color_text{
    "red",
    "green",
    "blue",
};

using Gameplay = std::vector<Cubes>;
using Games = std::vector<Gameplay>;

std::ostream &operator<<(std::ostream &os, const Cubes &cubes) {
  bool use_comma = false;
  if (cubes.red > 0) {
    os << "r=" << cubes.red;
    use_comma = true;
  }
  if (cubes.green > 0) {
    if (use_comma) {
      os << ", ";
    }
    os << "g=" << cubes.green;
    use_comma = true;
  }
  if (cubes.blue > 0) {
    if (use_comma) {
      os << ", ";
    }
    os << "b=" << cubes.blue;
  }

  return os;
}

std::ostream &operator<<(std::ostream &os, const Gameplay &gameplay) {
  for (const auto &cubes : gameplay) {
    os << " " << cubes << "\n";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Games &games) {
  for (auto count = 0; count < games.size(); count++) {
    os << "Game " << (count + 1) << "\n" << games[count] << "\n";
  }
  return os;
}

Games ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file\n");
  }

  Games games;
  std::string line;
  while (getline(infile, line)) {
    if (line.empty()) {
      throw std::logic_error("expected upper");
    }

    std::istringstream iss{line};
    std::string game_s, colon_s;
    int game_id;

    iss >> game_s >> game_id >> colon_s;

    if (!(game_s == "Game" && colon_s == ":")) {
      throw std::logic_error("invalid prefix");
    }

    Gameplay gameplay{};

    Cubes cubes{};
    while (true) {
      int quantity{};
      std::string color{};

      iss >> quantity >> color;

      bool is_comma = color[color.size() - 1] == ',';
      bool is_semicolon = color[color.size() - 1] == ';';
      bool is_end = !is_comma && !is_semicolon;

      if (!is_end) {
        color.pop_back();
      }

      if (color == "red") {
        cubes.red = quantity;
      } else if (color == "green") {
        cubes.green = quantity;
      } else if (color == "blue") {
        cubes.blue = quantity;
      } else {
        std::cout << color;
        throw std::logic_error("unexpected color: " + color);
      }

      if (is_end) {
        gameplay.push_back(cubes);
        break;
      }
      if (is_semicolon) {
        gameplay.push_back(cubes);
        cubes = Cubes();
      }
    }
    games.push_back(gameplay);
  }

  infile.close();

  return games;
}

constexpr auto kLimits = Cubes{
    .red = 12,
    .green = 13,
    .blue = 14,
};

bool IsPossible(const Gameplay &game_play) {
  for (const auto play : game_play) {
    if (play.red > kLimits.red) {
      return false;
    }
    if (play.green > kLimits.green) {
      return false;
    }
    if (play.blue > kLimits.blue) {
      return false;
    }
  }
  return true;
}

std::int64_t CalcPart1(const Games &games) {
  Timer t_main("calc p1");

  std::int64_t value{};
  for (auto pos = 0; pos < games.size(); pos++) {
    if (IsPossible(games[pos])) {
      value += pos + 1;
    }
  }

  return value;
}

std::int64_t CalcPart2(const Games &games) {
  Timer t_main("calc p2");
  return 0;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath\n");
  }

  Games games{ParseInput(argv[1])};

  // std::cout << games;

  auto result_p1 = CalcPart1(games);
  auto result_p2 = CalcPart2(games);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
