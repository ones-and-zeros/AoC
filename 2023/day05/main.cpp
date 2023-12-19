#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "timer.h"

namespace {

using Seeds = std::vector<std::int64_t>;

struct Map {
  std::int64_t destination;
  std::int64_t source;
  std::int64_t length;
};

std::ostream &operator<<(std::ostream &os, const Map &map) {
  os << map.destination << ", " << map.source << ", " << map.destination;
  return os;
}

using Mapping = std::vector<Map>;
using Mappings = std::vector<Mapping>;

struct Input {
  Seeds seeds;
  Mappings mappings;
};

std::int64_t Transform(const Mapping &mapping, std::int64_t source) {
  for (const auto &map : mapping) {
    if (source >= map.source && source < map.source + map.length) {
      return map.destination + (source - map.source);
    }
  }
  return source;
}

std::int64_t TransformRev(const Mapping &mapping, std::int64_t destination) {
  for (auto count = 0; count < mapping.size(); count++) {
    const auto &map = mapping[mapping.size() - (1 + count)];
    if (destination >= map.destination &&
        destination < map.destination + map.length) {
      return map.source + (destination - map.destination);
    }
  }
  return destination;
}

Input ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file\n");
  }

  Input input{};

  std::string line;
  {
    getline(infile, line);
    std::istringstream iss{line};
    std::string header{};

    iss >> header;
    assert(header == "seeds:");

    while (!iss.eof()) {
      std::int64_t numb{};
      iss >> numb;
      input.seeds.push_back(numb);
    }
  }

  Mapping mapping{};
  while (getline(infile, line)) {
    if (line.size() == 0) {
      if (mapping.size() > 0) {
        input.mappings.push_back(mapping);
      }
      mapping.clear();
      continue;
    }

    std::istringstream iss{line};
    char ch;
    iss >> ch;
    if (!std::isdigit(ch)) {
      continue;
    }
    iss.putback(ch);

    Map map{};
    iss >> map.destination >> map.source >> map.length;

    mapping.push_back(map);
  }

  if (mapping.size() > 0) {
    input.mappings.push_back(mapping);
  }

  infile.close();
  return input;
}

std::int64_t CalcPart1(const Input &input) {
  Timer t_main("calc p1");

  auto to_transform = input.seeds;

  std::int64_t value{};
  for (const auto &mapping : input.mappings) {
    for (auto &item : to_transform) {
      item = Transform(mapping, item);
    }
  }

  std::sort(to_transform.begin(), to_transform.end());

  return to_transform[0];
}

struct SeedRange {
  std::int64_t location;
  std::int64_t length;
};

using SeedRanges = std::vector<SeedRange>;

SeedRanges ParseSeedRanges(const Seeds &seeds) {
  SeedRanges ranges{};

  for (auto pos = 0; pos < seeds.size(); pos += 2) {
    ranges.push_back(SeedRange{seeds[pos + 0], seeds[pos + 1]});
  }
  return ranges;
}

bool IsWithin(const SeedRanges &ranges, std::int64_t seed) {
  for (const auto &range : ranges) {
    if ((seed >= range.location) && (seed < (range.location + range.length))) {
      return true;
    }
  }
  return false;
}

std::int64_t CalcPart2(const Input &input) {
  Timer t_main("calc p2");

  const auto seed_ranges = ParseSeedRanges(input.seeds);

  for (auto location = std::int64_t{1};
       location < std::numeric_limits<std::int64_t>::max(); location++) {
    auto item = location;

    // std::cout << "loc " << item;

    for (auto count = 0; count < input.mappings.size(); count++) {
      item = TransformRev(input.mappings[input.mappings.size() - (1 + count)],
                          item);
      // std::cout << " -> " << item;
    }
    // std::cout << "\n";

    if (IsWithin(seed_ranges, item)) {
      return location;
    }
  }
  throw std::logic_error("no seed found");
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath\n");
  }

  auto input = ParseInput(argv[1]);

  auto result_p1 = CalcPart1(input);
  auto result_p2 = CalcPart2(input);
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
