#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "timer.h"

namespace {

struct Point {
  std::size_t row;
  std::size_t col;
};

std::ostream &operator<<(std::ostream &os, const Point &point) {
  os << "{" << point.row << ", " << point.col << "}";
  return os;
}

template <class T> auto Difference(const T &a, const T &b) {
  if (a > b) {
    return a - b;
  }
  return b - a;
}

struct Image {
  auto GetRowSize() const noexcept { return rows.size(); }
  auto GetColSize() const noexcept { return rows.size() ? rows[0].size() : 0; }

  bool IsGalaxyAtPoint(const Point &point) const {
    if (!IsPointValid(point)) {
      return false;
    }
    return rows[point.row][point.col] == '#';
  }

  bool IsPointValid(const Point &point) const {
    return (point.row < GetRowSize()) && (point.col < GetColSize());
  }

  bool IsRowEmpty(std::size_t row) const {
    for (auto col = 0U; col < GetColSize(); col++) {
      if (IsGalaxyAtPoint({row, col})) {
        return false;
      }
    }

    return true;
  }

  bool IsColEmpty(std::size_t col) const {
    for (auto row = 0U; row < GetRowSize(); row++) {
      if (IsGalaxyAtPoint({row, col})) {
        return false;
      }
    }

    return true;
  }

  std::int64_t Distance(const Point &point_a, const Point &point_b,
                        const std::int64_t &expansion_factor) const {
    std::int64_t distance = 0;

    auto row_start = std::min(point_a.row, point_b.row);
    auto row_stop = std::max(point_a.row, point_b.row);
    for (auto row = row_start; row < row_stop; row++) {
      distance += IsRowEmpty(row) ? expansion_factor : 1;
    }

    auto col_start = std::min(point_a.col, point_b.col);
    auto col_stop = std::max(point_a.col, point_b.col);
    for (auto col = col_start; col < col_stop; col++) {
      distance += IsColEmpty(col) ? expansion_factor : 1;
    }

    return distance;
  }

  std::vector<std::string> rows;
};

std::ostream &operator<<(std::ostream &os, const Image &image) {
  for (auto row = 0U; row < image.GetRowSize(); row++) {
    for (auto col = 0U; col < image.GetColSize(); col++) {
      os << (image.IsGalaxyAtPoint({row, col}) ? '#' : '.');
    }
    os << '\n';
  }
  os << '\n';
  return os;
}

std::vector<Point> FindAllGalaxies(const Image &image) {
  std::vector<Point> galaxies{};

  for (auto row = 0U; row < image.GetRowSize(); row++) {
    for (auto col = 0U; col < image.GetColSize(); col++) {
      if (image.IsGalaxyAtPoint({row, col})) {
        galaxies.push_back({row, col});
      }
    }
  }
  return galaxies;
}

std::int64_t FindSumMinDistancesForPairs(const Image &image,
                                         const std::vector<Point> &points,
                                         const std::int64_t &expansion_factor) {

  std::int64_t sum{};

  for (auto pos_a = 0U; pos_a < points.size(); pos_a++) {
    for (auto pos_b = pos_a + 1; pos_b < points.size(); pos_b++) {
      sum += image.Distance(points[pos_a], points[pos_b], expansion_factor);
    }
  }
  return sum;
}

Image ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Image image{};

  std::string line;
  while (getline(infile, line)) {
    image.rows.push_back(line);
  }

  infile.close();

  return image;
}

std::int64_t CalcPart1(const Image &image) {
  Timer t_main("calc p1");

  return FindSumMinDistancesForPairs(image, FindAllGalaxies(image), 2);
}

std::int64_t CalcPart2(const Image &image) {
  Timer t_main("calc p2");

  return FindSumMinDistancesForPairs(image, FindAllGalaxies(image), 1'000'000);
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
