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

std::int64_t MinDistance(const Point &point_a, const Point &point_b) {
  return Difference(point_a.row, point_b.row) +
         Difference(point_a.col, point_b.col);
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

Image ExpandImage(const Image &original) noexcept {
  Image expanded{original};

  // expand empty rows
  for (auto row = 0U; row < expanded.GetRowSize(); row++) {
    auto galaxy_found_in_col = false;
    for (auto col = 0U; col < expanded.GetColSize(); col++) {
      const auto current_point = Point{row, col};
      if (expanded.IsGalaxyAtPoint(current_point)) {
        galaxy_found_in_col = true;
        break;
      }
    }
    if (!galaxy_found_in_col) {
      expanded.rows.insert(expanded.rows.begin() + row, expanded.rows[row]);
      row++;
    }
  }

  // expand empty columns
  for (auto col = 0U; col < expanded.GetColSize(); col++) {
    auto galaxy_found_in_row = false;
    for (auto row = 0U; row < expanded.GetRowSize(); row++) {
      const auto current_point = Point{row, col};
      if (expanded.IsGalaxyAtPoint(current_point)) {
        galaxy_found_in_row = true;
        break;
      }
    }
    if (!galaxy_found_in_row) {
      for (auto row = 0U; row < expanded.GetRowSize(); row++) {
        expanded.rows[row].insert(expanded.rows[row].begin() + col, '.');
      }
      col++;
    }
  }

  return expanded;
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

std::int64_t FindSumMinDistancesForPairs(const std::vector<Point> &points) {

  std::int64_t sum{};

  for (auto pos_a = 0U; pos_a < points.size(); pos_a++) {
    for (auto pos_b = pos_a + 1; pos_b < points.size(); pos_b++) {
      const auto min_distance = MinDistance(points[pos_a], points[pos_b]);
      sum += min_distance;

      // std::cout << points[pos_a] << "," << points[pos_b] << " " << min_distance
      //           << "\n";
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
    // std::istringstream iss{line};
    // std::string text;
    // int digits;
    // iss >> text >> digits;
  }

  infile.close();

  return image;
}

std::int64_t CalcPart1(const Image &image) {
  Timer t_main("calc p1");

  const auto expanded = ExpandImage(image);
  const auto galaxies = FindAllGalaxies(expanded);

  return FindSumMinDistancesForPairs(galaxies);
}

std::int64_t CalcPart2(const Image &image) {
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
