#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "timer.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "error, expected 1 argument for inputfile filepath\n";
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

  // input parse check
  for (const auto &line : input_lines) {
    std::cout << line << "\n";
  }
  std::cout << "\n";

  std::uint64_t total{0};
  {
    Timer t_main("calc");

    for (const auto &line : input_lines) {
      auto from_begin = line.begin();
      while (!isdigit(*from_begin)) {
        from_begin++;
      }

      auto from_end = line.end() - 1;
      while (!isdigit(*from_end)) {
        from_end--;
      }

      if (from_begin > from_end) {
        std::cout << "error, parse order\n";
        return 1;
      }

      std::string number_text{};
      number_text += *from_begin;
      number_text += *from_end;

      // std::cout << number_text << "\n";
      total += std::stoi(number_text);
    }
  }

  std::cout << "\n";
  std::cout << "part 1) : " << total << "\n";
  std::cout << "part 2) : "
            << "TODO"
            << "\n";
  std::cout << "\n";
}
