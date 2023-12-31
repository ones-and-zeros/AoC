#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "timer.h"

namespace {

struct Point {
  int row;
  int column;
};

bool operator<(const Point &lso, const Point &rso) {
  if (lso.row < rso.row) {
    return true;
  }
  if (lso.row > rso.row) {
    return false;
  }
  return lso.column < rso.column;
}

using Contraption = std::vector<std::string>;

Contraption ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Contraption contraption{};

  std::string line;
  while (getline(infile, line)) {
    contraption.push_back(line);
  }

  infile.close();

  return contraption;
}

enum class Dir : std::uint8_t {
  North = 0,
  South,
  East,
  West,
};

enum class Space : char {
  Empty = '.',
  MirrorLeft = '\\',
  MirrorRight = '/',
  SplitterHorizontal = '-',
  SplitterVertical = '|',
};

Dir NewDirMirrorLeft(const Dir &dir) {
  switch (dir) {
  case Dir::North:
    return Dir::West;

  case Dir::South:
    return Dir::East;

  case Dir::West:
    return Dir::North;

  case Dir::East:
    return Dir::South;
  }
  assert(false);
}

Dir NewDirMirrorRight(const Dir &dir) {
  switch (dir) {
  case Dir::North:
    return Dir::East;

  case Dir::South:
    return Dir::West;

  case Dir::West:
    return Dir::South;

  case Dir::East:
    return Dir::North;
  }
  assert(false);
}

struct Move {
  Point location;
  Dir dir;
};

Point NextPoint(const Move &move) {
  switch (move.dir) {
  case Dir::North:
    return {move.location.row - 1, move.location.column};
  case Dir::South:
    return {move.location.row + 1, move.location.column};
  case Dir::East:
    return {move.location.row, move.location.column + 1};
  case Dir::West:
    return {move.location.row, move.location.column - 1};
  }
  assert(false);
}

bool operator<(const Move &lso, const Move &rso) {
  if (lso.location < rso.location) {
    return true;
  }
  if (rso.location < lso.location) {
    return false;
  }
  return lso.dir < rso.dir;
}

std::set<Point> Travel(const Contraption &contraption, const Move &move) {
  if (move.location.row < 0 || move.location.row >= contraption.size()) {
    return {};
  }

  if (move.location.column < 0 ||
      move.location.column >= contraption[0].size()) {
    return {};
  }

  static std::set<Move> moves_made{};

  if (moves_made.find(move) != moves_made.end()) {
    return {};
  }
  moves_made.insert(move);

  std::set<Point> energized{};
  energized.insert(move.location);

  const Space current_space =
      static_cast<Space>(contraption[move.location.row][move.location.column]);

  switch (current_space) {
  case Space::Empty: {
    energized.merge(Travel(contraption, {NextPoint(move), move.dir}));
    break;
  }
  case Space::MirrorLeft: {
    auto new_dir = NewDirMirrorLeft(move.dir);
    energized.merge(
        Travel(contraption, {NextPoint({move.location, new_dir}), new_dir}));
    break;
  }
  case Space::MirrorRight: {
    auto new_dir = NewDirMirrorRight(move.dir);
    energized.merge(
        Travel(contraption, {NextPoint({move.location, new_dir}), new_dir}));
    break;
  }
  case Space::SplitterHorizontal: {
    if (move.dir == Dir::East || move.dir == Dir::West) {
      energized.merge(Travel(contraption, {NextPoint(move), move.dir}));
      break;
    }
    energized.merge(Travel(contraption,
                           {NextPoint({move.location, Dir::East}), Dir::East}));
    energized.merge(Travel(contraption,
                           {NextPoint({move.location, Dir::West}), Dir::West}));
    break;
  }
  case Space::SplitterVertical: {
    if (move.dir == Dir::North || move.dir == Dir::South) {
      energized.merge(Travel(contraption, {NextPoint(move), move.dir}));
      break;
    }
    energized.merge(Travel(
        contraption, {NextPoint({move.location, Dir::North}), Dir::North}));
    energized.merge(Travel(
        contraption, {NextPoint({move.location, Dir::South}), Dir::South}));
    break;
  }
  }
  return energized;
}

std::int64_t CalcPart1(const Contraption &contraption) {
  Timer t_main("calc p1");

  const auto energized = Travel(contraption, {{0, 0}, Dir::East});

  for (auto row = 0; row < contraption.size(); row++) {
    for (auto col = 0; col < contraption[0].size(); col++) {
      if (energized.find({row, col}) != energized.end()) {
        std::cout << "#";
        continue;
      }
      std::cout << contraption[row][col];
    }
    std::cout << "\n";
  }

  return energized.size();
}

std::int64_t CalcPart2(const Contraption &contraption) {
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
