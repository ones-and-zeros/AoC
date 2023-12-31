#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

namespace {

using Steps = std::vector<std::string>;

struct BoxEntry {
  std::string label;
  int focal_length;
};

using Box = std::vector<BoxEntry>;
using Boxes = std::array<Box, 256>;

Steps ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Steps steps{};

  std::string line;

  std::string current_step{};
  while (getline(infile, line)) {

    for (auto ch : line) {
      if (ch != ',') {
        current_step += ch;
        continue;
      }
      if (current_step.size() > 0) {
        steps.push_back(current_step);
        current_step.clear();
      }
    }
  }
  if (current_step.size() > 0) {
    steps.push_back(current_step);
  }

  infile.close();

  return steps;
}

std::uint8_t ToHash(const std::string &step) {
  std::uint8_t value = 0;

  for (auto ch : step) {
    value = static_cast<std::uint8_t>(((std::uint32_t{value} + ch) * 17));
  }
  return value;
}

void RemoveFromBox(Box &box, const std::string &label) {
  for (auto it = box.begin(); it != box.end(); it++) {
    if (it->label == label) {
      box.erase(it);
      break;
    }
  }
}

void AddToBox(Box &box, const BoxEntry &box_entry) {
  for (auto it = box.begin(); it != box.end(); it++) {
    if (it->label == box_entry.label) {
      *it = box_entry;
      return;
    }
  }
  box.push_back(box_entry);
}

std::int64_t CalcPart1(const Steps &steps) {
  Timer t_main("calc p1");
  std::int64_t sum = 0;

  for (const auto &step : steps) {

    const auto value = ToHash(step);
    // std::cout << step << ": " << int(value) << "\n";

    sum += value;
  }
  return sum;
}

std::int64_t Score(const Boxes &boxes) {
  std::int64_t total = 0;

  for (auto box_offset = 0; box_offset < boxes.size(); box_offset++) {
    auto &box = boxes[box_offset];

    if (box.size() > 0) {
      // std::cout << "box " << box_offset << "\n";

      for (auto slot_offset = 0; slot_offset < box.size(); slot_offset++) {
        const auto &lense = box[slot_offset];
        const auto value = std::int64_t{box_offset + 1} *
                           std::int64_t{slot_offset + 1} * lense.focal_length;
        total += value;

        // std::cout << " " << lense.label << " " << lense.focal_length;
        // std::cout << ": " << value << "\n";
      }
    }
  }

  return total;
}

std::int64_t CalcPart2(const Steps &steps) {
  Timer t_main("calc p2");

  Boxes boxes{};

  for (const auto &step : steps) {
    std::istringstream iss{step};
    std::string label{};
    char next_char;

    while (iss >> next_char && std::isalpha(next_char)) {
      label += next_char;
    }
    const auto box_offset = ToHash(label);

    if (next_char == '-') {
      RemoveFromBox(boxes[box_offset], label);
      continue;
    }

    int focal_length{};
    iss >> focal_length;

    if (next_char == '=') {
      AddToBox(boxes[box_offset], {label, focal_length});
      continue;
    }
    assert(false);
  }

  return Score(boxes);
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
