#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

#include "timer.h"

struct Point {
  int x;
  int y;
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << "{" << p.x << "," << p.y << "}";
  return os; 
}

bool operator==(const Point& a, const Point& b) {
  return {a.x == b.x && a.y == b.y}; 
}

bool operator!=(const Point& a, const Point& b) {
  return !(a == b);
}

Point operator+(const Point& a, const Point& b) {
  return {a.x + b.x, a.y + b.y}; 
}

Point operator-(const Point& a, const Point& b) {
  return {a.x - b.x, a.y - b.y}; 
}

using Shape = std::vector<Point>;
using Shapes = std::vector<Shape>;


int GetYSize(const Shapes& shapes) {
  int y_max = std::numeric_limits<int>::min();

  for (const auto& shape : shapes) {
    for (const auto& point : shape) {
      y_max = std::max(y_max, point.y);
    }
  }
  return y_max + 1;
}

constexpr char rock{'#'};
constexpr char air{'.'};
constexpr char sand{'o'};
constexpr Point sand_start{500,0};

struct World {
  using Map = std::vector<std::string>;
  static constexpr std::size_t x_size{1000};


  explicit World(const Shapes& shapes, const bool infinite_floor_) {
    const auto y_size = GetYSize(shapes);

    map = CreateBlankMap(y_size);
    for (const auto& shape : shapes) {
      AddShape(shape);
    }

    infinite_floor = infinite_floor_;
    if (infinite_floor) {
      map.push_back(std::string(x_size, {air}));
      map.push_back(std::string(x_size, {rock}));
    }

    sand_count = 0;
  }

  Map CreateBlankMap(const int y_size) {
    Map map_;
    for (int y_pos = 0; y_pos < y_size; y_pos++) {
      map_.push_back(std::string(x_size, {air}));
    }
    return map_;
  }

  void MarkMap(const Point& p, const char c) {
    map[p.y][p.x] = c;    
  }

  void AddShape(const Shape& shape) {
    for (std::size_t idx = 1; idx < shape.size(); idx++) {
      DrawLine(shape[idx - 1], shape[idx], rock);
    }
  }

  void DrawLine(const Point& a, const Point& b, char c) {
    const auto path = b - a;
    Point step{};

    if (path.y == 0 && path.x != 0) {
      step = path.x < 0 ? Point{-1,0} : Point{1,0};
    } else if (path.x == 0 && path.y != 0) {
      step = path.y < 0 ? Point{0,-1} : Point{0,1};
    } else {
      // no path
      std::cout << std::endl;
      throw std::logic_error("expected unique point");
    }

    auto pos = a;
    while(pos != b) {
      MarkMap(pos, rock);
      pos = pos + step;
    }
    MarkMap(pos, rock);
  }

  std::optional<char> GetChar(const Point& p) {
    if ( (p.x < 0) ||
         (p.x >= map[0].size()) ||
         (p.y >= map.size()) ) {
      return std::nullopt;
    }
    return map[p.y][p.x];
  }

  std::optional<bool> IsAvail(const Point& p) {
    auto result_char = GetChar(p);
    if (!result_char.has_value()) {
      return std::nullopt;
    }
    return (result_char.value() == air);
  }

  static constexpr std::array<Point, 3> fall_steps {
    Point{ 0,1},
    Point{-1,1},
    Point{ 1,1}
  };


  bool AddSandGrain() {
    Point sand_pos{sand_start};

    {
      const auto first_avail_result = IsAvail(sand_pos);
      if ( !first_avail_result.has_value() ||
          !first_avail_result.value() ) {
        return false;
      }
    }

    while (1) {
      Point loop_start = sand_pos;
      for (const auto& fall_step : fall_steps) {
        const Point next = sand_pos + fall_step;
        const auto result_avail = IsAvail(next);
        if ( !result_avail.has_value() ) {
          return false;
        }
        if (result_avail.value()) {
          sand_pos = next;
          break;
        }
      }
      if (loop_start == sand_pos) {
        break;
      }
    }

    sand_count++;
    MarkMap(sand_pos, sand);
    return true;
  }

  Map map;
  std::size_t sand_count;
  bool infinite_floor;
};

std::ostream& operator<<(std::ostream& os, const World& world) {
  os << "    " << std::string( sand_start.x, {' '}) << "|\n";
  for (std::size_t y = 0; y < world.map.size(); y++ ) {
    os << std::setfill(' ') << std::setw(3) << y;;
    os << " " << world.map[y] << "\n";
  }
  return os;
}

Shape ParseShape(const std::string& line) {
  Shape shape;

  std::istringstream iss{line};
  while(iss.rdbuf()->in_avail()) {
    int x,y;
    char comma_c;
    iss >> x >> comma_c >> y;
    if(comma_c != ',') {
      std::cout << std::endl;
      throw std::logic_error("expected comma");
    }
    shape.push_back({x,y});

    if(iss.rdbuf()->in_avail()) {
      std::string arrow_s;
      iss >> arrow_s;
      if(arrow_s != "->") {
        std::cout << std::endl;
        throw std::logic_error("expected '->'");
      }
    }
  }

  return shape;
}

std::size_t Calc(World world) {
  while(world.AddSandGrain()) {
    ;
  }
  std::cout << world << "\n\n";

  return world.sand_count;
}

std::size_t CalcPart1(const Shapes& shapes) {
  return Calc(World(shapes, false));
}

std::size_t CalcPart2(const Shapes& shapes) {
  return Calc(World(shapes, true));
}

int main()
{
  Timer t_main("main");

  Shapes shapes;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      shapes.push_back(ParseShape(line));
    }

    infile.close();
  }
  else {
    std::cout << std::endl;
    throw std::logic_error("unable to open input file");
  }
  std::cout << "\n";

  std::size_t result_p1{0};
  std::size_t result_p2{0};

  {
    Timer t_main("calc");

    result_p1 = CalcPart1(shapes);
    result_p2 = CalcPart2(shapes);
  }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
