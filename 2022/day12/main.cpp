#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "timer.h"

using Heightmap = std::vector<std::string>;

struct Point {
  int x;
  int y;
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os <<  "{" << p.x << "," << p.y << "}";
  return os;
}

constexpr Point operator+(const Point& a, const Point& b) {
  return {a.x + b.x, a.y + b.y};
}

constexpr Point operator-(const Point& a, const Point& b) {
  return {a.x - b.x, a.y - b.y};
}

constexpr bool operator==(const Point& a, const Point& b) {
  return (a.x == b.x) && (a.y == b.y);
}

constexpr bool operator<(const Point& a, const Point& b) {
  return (a.x < b.x) ||
         ((a.x == b.x) && (a.y < b.y));
}

constexpr std::array<Point, 4> neighbors_relative {{{-1,0}, {1,0}, {0,-1}, {0,1}}};

namespace {

using PointsVisited = std::set<Point>;

Heightmap heightmap;
std::size_t x_size = 0;
std::size_t y_size = 0;
Point end_point{};

std::size_t least_steps{0};
std::size_t least_steps_series{0};
PointsVisited points_visited;
std::map<Point, std::size_t> steps_per_point;

}

Point FindAPoint(const char c) {
  for (int y = 0; y < y_size; ++y) {
    for (int x = 0; x < x_size; ++x) {
      if( heightmap[y][x] == c)
        return {x,y};
    }
  }
  throw std::logic_error("cannot find c");
}

std::vector<Point> FindAllPoints(const char c) {
  std::vector<Point> points;
  for (int y = 0; y < y_size; ++y) {
    for (int x = 0; x < x_size; ++x) {
      if( heightmap[y][x] == c)
        points.push_back({x,y});
    }
  }
  return points;
}

char GetChar(const Point point) {
  return heightmap[point.y][point.x];
}

bool CanTravelTo(const Point& from, const Point& to) {
  auto c_from = GetChar(from);
  auto c_to = GetChar(to);

  if(c_from == 'S') {
    c_from = 'a';
  }
  if(c_to == 'E') {
    c_to = 'z';
  }

  return (c_from + 1 >= c_to);
}

std::vector<Point> GetNeighbors(Point location) {
  std::vector<Point> neighbors;

  for(auto neighbor : neighbors_relative) {
    neighbor = location + neighbor;
    if (neighbor.x < 0 || neighbor.y < 0 ||
        neighbor.x >= x_size || neighbor.y >= y_size ) {
      continue;
    }
    if (!CanTravelTo(location, neighbor)) {
      continue;
    }
    neighbors.push_back(neighbor);
  }

  return neighbors;
}

constexpr int DistMetric(const Point& a, const Point& b) {
  const auto p = a - b;

  return (p.x+p.y);
}

void Step(Point position, std::size_t steps) {
  if(steps_per_point.end() == steps_per_point.find(position)) {
    //
  } else if(steps_per_point[position] <= steps){
    return;
  }
  steps_per_point[position] = steps;

  if(steps > least_steps) {
    return;
  }
  if(end_point == position) {
    if (steps < least_steps) {
      least_steps = steps;
//      std::cout << " found steps " << steps << "\n";
    }
    return;
  }
  if(points_visited.end() != points_visited.find(position)) {
    return;
  }

  points_visited.insert(position);
  steps++;

  auto neighbors = GetNeighbors(position);
  std::sort(
    neighbors.begin(),
    neighbors.end(),
    [](const Point& a, const Point& b){
      const auto a_to_end = DistMetric(a, end_point);
      const auto b_to_end = DistMetric(b, end_point);

      if(a_to_end == b_to_end) {
        return GetChar(b) < GetChar(a);
      }
      return a_to_end < b_to_end;
  });

  for(const auto& neighbor : neighbors) {  
    Step(neighbor, steps);
  }

  steps--;
  points_visited.erase(position);
}

std::size_t StepsToEnd(Point start) {
  least_steps = least_steps_series;
  points_visited.clear();
  steps_per_point.clear();
  end_point = FindAPoint('E');

  Step(start, 0);
  return least_steps;
}

std::size_t CalcPart1() {
  least_steps_series = ~0;
  return StepsToEnd(FindAPoint('S'));
}

std::size_t CalcPart2() {
  least_steps_series = ~0;

  const auto starting_points = FindAllPoints('a');
  for(const auto& start : starting_points) {
    least_steps_series = StepsToEnd(start);
  }
  return least_steps_series;
}

int main()
{
  Timer t_main("main");

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      heightmap.push_back(line);
    }

    x_size = heightmap[0].size();
    y_size = heightmap.size();

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& line : heightmap) {
  //   std::cout << line << "\n";
  // }
  // std::cout << "\n";

  std::size_t result_p1;
  std::size_t result_p2;

  {
    Timer t_main("calc");

    result_p1 = CalcPart1();
    result_p2 = CalcPart2();
  }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
