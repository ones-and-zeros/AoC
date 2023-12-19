#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

namespace {

struct Race {
  std::int64_t time;
  std::int64_t distance;
};

using Races = std::vector<Race>;

Races ParseInput1(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  std::string line_time;
  std::string line_distance;
  if (!getline(infile, line_time) || !getline(infile, line_distance)) {
    throw std::logic_error("missing line(s)");
  }
  std::istringstream iss_time{line_time};
  std::istringstream iss_distance{line_distance};

  std::string prefix{};
  iss_time >> prefix;
  assert(prefix == "Time:");

  iss_distance >> prefix;
  assert(prefix == "Distance:");

  Races races{};
  int time, distance;
  while (iss_time >> time && iss_distance >> distance) {
    races.push_back(Race{time, distance});
  }

  infile.close();

  return races;
}

Races ParseInput2(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  std::string line_time;
  std::string line_distance;
  if (!getline(infile, line_time) || !getline(infile, line_distance)) {
    throw std::logic_error("missing line(s)");
  }
  std::istringstream iss_time{line_time};
  std::istringstream iss_distance{line_distance};

  std::string prefix{};
  iss_time >> prefix;
  assert(prefix == "Time:");

  iss_distance >> prefix;
  assert(prefix == "Distance:");

  Races races{};
  std::string time{}, distance{};
  std::string time_snip{}, distance_snip{};

  while (iss_time >> time_snip && iss_distance >> distance_snip) {
    time += time_snip;
    distance += distance_snip;
  }

  std::cout << time << "\n";
  std::cout << distance << "\n";

  races.push_back(Race{std::stol(time), std::stol(distance)});

  infile.close();

  return races;
}

std::int64_t CalcDistance(std::int64_t time_held, std::int64_t total_time) {
  return time_held * (total_time - time_held);
}

std::int64_t CalcPartX(const Races &races) {
  Timer t_main("calc pX");

  std::int64_t value{1};
  for (const auto &race : races) {
    std::int64_t winning_combos{};
    for (std::int64_t hold_time = 1; hold_time < race.time; hold_time++) {
      if (CalcDistance(hold_time, race.time) > race.distance) {
        winning_combos++;
      }
    }
    value *= winning_combos;
  }

  return value;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath");
  }

  auto input1 = ParseInput1(argv[1]);
  auto input2 = ParseInput2(argv[1]);

  // std::cout << games;

  auto result_p1 = CalcPartX(input1);
  auto result_p2 = CalcPartX(input2);

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
