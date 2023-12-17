#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "timer.h"

int FirstDigit(const std::string &line) {
  for (auto pos = 0; pos < line.size(); pos++) {
    if (isdigit(line[pos])) {
      return std::stoi(line.substr(pos, 1));
    }
  }
  return 0;
}

int LastDigit(const std::string &line) {
  for (auto ch = line.rbegin(); ch != line.rend(); ch++) {
    if (isdigit(*ch)) {
      std::string text;
      text += *ch;
      return std::stoi(text);
    }
  }
  return 0;
}

static constexpr auto kMinCharsForNumb{3};

static const std::array<std::string, 10> text_numbers{
    "zero", "one", "two",   "three", "four",
    "five", "six", "seven", "eight", "nine",
};

std::optional<int> MaybeTextNumber(const std::string& text) {
  for (auto count = 0; count < text_numbers.size(); count++) {
    if (text == text_numbers[count]) {
      return count;
    }
  }
  return std::nullopt;
}

std::optional<int> MaybeNumber(const std::string& text) {
  if (isdigit(text[0])) {
    return std::stoi(text.substr(0, 1));
  }
  for (auto length = kMinCharsForNumb; length <= text.size(); length++) {
    auto maybe_number = MaybeTextNumber(text.substr(0, length));
    if (maybe_number.has_value()) {
      return maybe_number.value();
    }
  }
  return std::nullopt;
}

int FirstNumber(const std::string &line) {
  for (auto pos = 0; pos < line.size(); pos++) {
    auto maybe_number = MaybeNumber(line.substr(pos, line.length() - pos));
    if (maybe_number.has_value()) {
      return maybe_number.value();
    }
  }
  return 0;
}

int LastNumber(const std::string &line) {
  for (auto rpos = 0; rpos < line.size(); rpos++) {
    auto pos = line.size() - (1 + rpos);
    auto maybe_number = MaybeNumber(line.substr(pos, line.length() - pos));
    if (maybe_number.has_value()) {
      return maybe_number.value();
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "error, expected 1 argument for input filepath\n";
    return 1;
  }

  std::ifstream infile{argv[1]};
  if (!infile.is_open()) {
    std::cout << "error, unable to open input file\n";
    return 1;
  }

  std::vector<std::string> input_lines{};

  {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      input_lines.push_back(line);
    }

    infile.close();
  }
  std::cout << "\n";

  // // input parse check
  // for (const auto &line : input_lines) {
  //   std::cout << line << "\n";
  // }
  // std::cout << "\n";

  std::uint64_t total_p1{0};
  {
    Timer t_main("calc p1");

    for (const auto &line : input_lines) {
      total_p1 += (10 * FirstDigit(line)) + LastDigit(line);
    }
  }

  std::uint64_t total_p2{0};
  {
    Timer t_main("calc p2");

    for (const auto &line : input_lines) {
      total_p2 += (10 * FirstNumber(line)) + LastNumber(line);
    }
  }

  std::cout << "\n";
  std::cout << "part 1) : " << total_p1 << "\n";
  std::cout << "part 2) : " << total_p2 << "\n";
  std::cout << "\n";
}
