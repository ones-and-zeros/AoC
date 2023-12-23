#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <vector>

#include "timer.h"

namespace {

// | is a vertical pipe connecting north and south.
// - is a horizontal pipe connecting east and west.
// L is a 90-degree bend connecting north and east.
// J is a 90-degree bend connecting north and west.
// 7 is a 90-degree bend connecting south and west.
// F is a 90-degree bend connecting south and east.
// . is ground; there is no pipe in this tile.
// S is the starting position of the animal; there is a pipe on this tile, but
// your sketch doesn't show what shape the pipe has.

constexpr auto kPipeVertical{'|'};
constexpr auto kPipeHorizontal{'-'};
constexpr auto kPipeBendNE{'L'};
constexpr auto kPipeBendNW{'J'};
constexpr auto kPipeBendSW{'7'};
constexpr auto kPipeBendSE{'F'};
constexpr auto kGround{'.'};
constexpr auto kStart{'S'};

struct Point {
  std::int32_t row;
  std::int32_t col;
};

Point operator+(Point point_a, Point point_b) {
  return Point{{point_a.row + point_b.row}, {point_a.col + point_b.col}};
}

bool operator<(const Point &point_a, const Point &point_b) {
  if (point_a.row < point_b.row) {
    return true;
  }
  if (point_a.row > point_b.row) {
    return false;
  }
  return (point_a.col < point_b.col);
}

struct PipeMap {
  char GetChar(const Point &point) const {
    return pipe_map[point.row][point.col];
  }

  void Push(const std::string &row_of_data) {
    if (pipe_map.size() > 0) {
      assert(pipe_map.rbegin()->size() == row_of_data.size());
    }
    pipe_map.push_back(row_of_data);
  }

  auto GetRowQty() const { return pipe_map.size(); }

  auto GetColQty() const { return pipe_map[0].size(); }

  std::vector<std::string> pipe_map;
};

struct Location {
  Point point;
  std::int64_t steps;
};

bool operator<(const Location &loc_a, const Location &loc_b) {
  if (loc_a.steps < loc_b.steps) {
    return true;
  }
  if (loc_a.steps > loc_b.steps) {
    return false;
  }
  return (loc_a.point < loc_b.point);
}

struct Status {
  std::map<Point, std::int64_t> steps;
  std::set<Point> unvisited;
  std::set<Point> visited;
};

PipeMap ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  PipeMap pipe_map{};

  std::string line;
  while (getline(infile, line)) {
    pipe_map.Push(line);
  }

  infile.close();

  return pipe_map;
}

Point FindStart(const PipeMap &pipe_map) {
  for (auto row = 0; row < pipe_map.GetRowQty(); row++) {
    for (auto col = 0; col < pipe_map.GetColQty(); col++) {
      const auto point = Point{row, col};
      if (pipe_map.GetChar(point) == kStart) {
        return point;
      }
    }
  }
  assert(false);
}

std::optional<Point> MoveNorth(const PipeMap &pipe_map, Point current) {
  if (current.row == 0) {
    return std::nullopt;
  }

  switch (pipe_map.GetChar(current)) {
  case kStart:
  case kPipeVertical:
  case kPipeBendNE:
  case kPipeBendNW:
    break;

  default:
    return std::nullopt;
  }

  const auto next_point = current + Point{-1, 0};
  switch (pipe_map.GetChar(next_point)) {
  case kStart:
  case kPipeVertical:
  case kPipeBendSW:
  case kPipeBendSE:
    break;

  default:
    return std::nullopt;
  }
  return next_point;
}

std::optional<Point> MoveSouth(const PipeMap &pipe_map, Point current) {
  if (current.row == pipe_map.GetRowQty() - 1) {
    return std::nullopt;
  }

  switch (pipe_map.GetChar(current)) {
  case kStart:
  case kPipeVertical:
  case kPipeBendSE:
  case kPipeBendSW:
    break;

  default:
    return std::nullopt;
  }

  const auto next_point = current + Point{1, 0};
  switch (pipe_map.GetChar(next_point)) {
  case kStart:
  case kPipeVertical:
  case kPipeBendNW:
  case kPipeBendNE:
    break;

  default:
    return std::nullopt;
  }
  return next_point;
}

std::optional<Point> MoveEast(const PipeMap &pipe_map, Point current) {
  if (current.col == pipe_map.GetColQty() - 1) {
    return std::nullopt;
  }

  switch (pipe_map.GetChar(current)) {
  case kStart:
  case kPipeHorizontal:
  case kPipeBendNE:
  case kPipeBendSE:
    break;

  default:
    return std::nullopt;
  }

  const auto next_point = current + Point{0, 1};
  switch (pipe_map.GetChar(next_point)) {
  case kStart:
  case kPipeHorizontal:
  case kPipeBendNW:
  case kPipeBendSW:
    break;

  default:
    return std::nullopt;
  }
  return next_point;
}

std::optional<Point> MoveWest(const PipeMap &pipe_map, Point current) {
  if (current.col == 0) {
    return std::nullopt;
  }

  switch (pipe_map.GetChar(current)) {
  case kStart:
  case kPipeHorizontal:
  case kPipeBendNW:
  case kPipeBendSW:
    break;

  default:
    return std::nullopt;
  }

  const auto next_point = current + Point{0, -1};
  switch (pipe_map.GetChar(next_point)) {
  case kStart:
  case kPipeHorizontal:
  case kPipeBendNE:
  case kPipeBendSE:
    break;

  default:
    return std::nullopt;
  }
  return next_point;
}

void Print(const PipeMap &pipe_map, const Status &status) {
  for (auto row = 0; row < pipe_map.GetRowQty(); row++) {
    std::cout << '\n';
    for (auto col = 0; col < pipe_map.GetColQty(); col++) {
      const auto result_find = status.steps.find(Point{row, col});
      if (result_find == status.steps.end()) {
        std::cout << '.';
        continue;
      }
      auto digits = result_find->second;
      if (digits > 9) {
        std::cout << '+';
        continue;
      }
      std::cout << digits;
    }
  }
  std::cout << '\n';
}

void Visit(const PipeMap &pipe_map, Status &status, Point location) {
  auto result_find = status.visited.find(location);
  if (result_find != status.visited.end()) {
    return;
  }

  // Print(pipe_map, status);

  std::vector<Point> available{};

  const auto result_north = MoveNorth(pipe_map, location);
  if (result_north.has_value()) {
    available.push_back(result_north.value());
  }

  const auto result_south = MoveSouth(pipe_map, location);
  if (result_south.has_value()) {
    available.push_back(result_south.value());
  }

  const auto result_east = MoveEast(pipe_map, location);
  if (result_east.has_value()) {
    available.push_back(result_east.value());
  }

  const auto result_west = MoveWest(pipe_map, location);
  if (result_west.has_value()) {
    available.push_back(result_west.value());
  }

  const auto next_step = status.steps[location] + 1;

  for (auto &next : available) {
    if (status.visited.find(next) != status.visited.end()) {
      continue;
    }
    if (status.unvisited.find(next) == status.unvisited.end()) {
      status.unvisited.insert(next);
      status.steps[next] = std::numeric_limits<std::int64_t>::max();
    }
    if (status.steps[next] > next_step) {
      status.steps[next] = next_step;
    }
  }

  status.visited.insert(location);
  status.unvisited.erase(location);

  if (status.unvisited.size() > 0) {
    // visit the lowest step count that is unvisted
    std::map<Point, std::int64_t> to_visit{};
    for (const auto &next_visit : status.unvisited) {
      to_visit[next_visit] = status.steps[next_visit];
    }
    using PairType = decltype(to_visit)::value_type;
    auto pr = std::min_element(std::begin(to_visit), std::end(to_visit),
                               [](const PairType &p1, const PairType &p2) {
                                 return p1.second < p2.second;
                               });

    Visit(pipe_map, status, pr->first);
  }
}

std::int64_t CalcPart1(const PipeMap &pipe_map) {
  Timer t_main("calc p1");

  Status status{};
  const auto start_pos = FindStart(pipe_map);
  status.steps[start_pos] = 0;
  Visit(pipe_map, status, start_pos);

  // Print(pipe_map, status);

  using PairType = decltype(status.steps)::value_type;
  auto it = std::max_element(std::begin(status.steps), std::end(status.steps),
                             [](const PairType &p1, const PairType &p2) {
                               return p1.second < p2.second;
                             });
  return it->second;
}

std::int64_t CalcPart2(const PipeMap &pipe_map) {
  Timer t_main("calc p2");

  return 0;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath");
  }

  auto input = ParseInput(argv[1]);

  // std::cout << games;

  auto result_p1 = CalcPart1(input);
  auto result_p2 = CalcPart2(input);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
