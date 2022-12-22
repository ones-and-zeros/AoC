#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "timer.h"

struct Move {
  char cmd;
  int qty;
};

using Moves = std::vector<Move>;

struct Point {
  int x;
  int y;
};

bool operator<(const Point& a, const Point& b) {
  if(a.x == b.x) {
    return a.y < b.y;
  }
  return a.x < b.x;
}

using Rope = std::vector<Point>;

enum class Direction {
  Up,
  Down,
  Left,
  Right
};

constexpr Direction ToDirection(const char cmd) {
  switch(cmd) {
    case 'U':
      return Direction::Up;
    case 'D':
      return Direction::Down;
    case 'L':
      return Direction::Left;
    case 'R':
      return Direction::Right;
  }
  throw std::logic_error("unknownn command");
}

constexpr void AdjustTail(const Point& head, Point& tail) {
  int x_diff = head.x - tail.x;
  int y_diff = head.y - tail.y;

  if (std::abs(x_diff) > 1) {
    tail.x += (x_diff < 0) ? -1 : 1;
    if (y_diff != 0) {
      tail.y += (y_diff < 0) ? -1 : 1;
    }
  }

  else if (std::abs(y_diff) > 1) {
    tail.y += (y_diff < 0) ? -1 : 1;
    if (x_diff != 0) {
      tail.x += (x_diff < 0) ? -1 : 1;
    }
  }
}

Rope MakeMove(Rope& rope, const Direction dir) {
  switch(dir) {
    case Direction::Up:
      rope[0].y++;
      break;
    case Direction::Down:
      rope[0].y--;
      break;
    case Direction::Left:
      rope[0].x--;
      break;
    case Direction::Right:
      rope[0].x++;
      break;
    default:
      throw std::logic_error("invalid direction");
  }

  for (auto head = rope.begin(); (head + 1) != rope.end(); head++) {
    AdjustTail(*head, *(head + 1));
  }

  return rope;
}

template<std::size_t N>
std::size_t Calc(const Moves& moves) {
  Rope rope{N, {0,0}};

  std::set<Point> tail_history{};
  tail_history.insert(rope[N - 1]);

  for(const auto& move : moves) {
    for(std::size_t count=0; count < move.qty; count++) {    
      rope = MakeMove(rope, ToDirection(move.cmd));
      tail_history.insert(rope[N - 1]);

      // const auto& tail = rope[N - 1];
      // std::cout << tail.x << "," << tail.y << "\n";
    }
  }

  return tail_history.size();
}

std::size_t CalcPart1(const Moves& moves) {
  return Calc<2>(moves);
}

std::size_t CalcPart2(const Moves& moves) {
  return Calc<10>(moves);
}

int main()
{
  Timer t_main("main");

  Moves moves;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      std::istringstream iss{line};
      char cmd;
      int qty;

      iss >> cmd >> qty;

      moves.push_back({cmd, qty});
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& move : moves) {
  //   std::cout << move.cmd << " " << move.qty << "\n";
  // }
  // std::cout << "\n";

  std::size_t result_p1{};
  std::size_t result_p2{};

  {
    Timer t_main("calc");

    result_p1 = CalcPart1(moves);
    result_p2 = CalcPart2(moves);
  }

  std::cout << "\n";
  std::cout << "part 1) " << result_p1 << "\n";
  std::cout << "part 2) " << result_p2 << "\n";
  std::cout << "\n";
}
