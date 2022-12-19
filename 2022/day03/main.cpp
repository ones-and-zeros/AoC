#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "timer.h"

using RutsackContents = std::string;
using RutsackList = std::vector<RutsackContents>;

char RutsackParse(const RutsackContents& contents) {
  const std::size_t compartment_size = contents.size() / 2;
  const auto b_begin = contents.begin() + compartment_size;
  const std::string& compartment_a{contents.begin(), b_begin};
  const std::string& compartment_b{b_begin, contents.end() };

  for(const auto& item : compartment_a) {
    const auto result = compartment_b.find(item);
    if(result != std::string::npos) {
      return compartment_b[result];
    }
  }

  return '0';
}

char BunchParse(std::array<const RutsackContents*, 3> bunch) {
  const RutsackContents& elf_a = *bunch[0];
  const RutsackContents& elf_b = *bunch[1];
  const RutsackContents& elf_c = *bunch[2];

  for (const auto& item : elf_a) {
    const auto result_b = elf_b.find(item);
    if(result_b == std::string::npos) continue;

    const auto result_c = elf_c.find(item);
    if(result_c == std::string::npos) continue;

    return item;
  }

  return '0';
}

constexpr std::uint32_t ItemValue(const char c) {
  if (!std::isalpha(c)) return 0;
  if (std::islower(c)) return c - 'a' + 1;
  return c - 'A' + 27;
}

int main()
{
  Timer t_main("main");

  RutsackList rutsack_list;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      rutsack_list.push_back(line);
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& line : packing_list) {
  //   std::cout << line << "\n";
  // }

  std::uint64_t sum_1 = 0;
  std::uint64_t sum_2 = 0;

  {
    Timer t_main("calc");

    // part 1
    for (const auto& rutsack : rutsack_list) {
      const auto result = RutsackParse(rutsack);
      const auto value = ItemValue(result);
      sum_1 += value;
//      std::cout << result << " (" << value << ")\n";
    }

    // part 2
    if(rutsack_list.size() % 3 != 0) {
      throw std::logic_error("lists must be div 3");
    }
    for (std::size_t idx = 0; idx < rutsack_list.size(); idx += 3) {
      const auto result = BunchParse(
            { &rutsack_list[idx],
              &rutsack_list[idx + 1],
              &rutsack_list[idx + 2] });
      const auto value = ItemValue(result);
      sum_2 += value;      
//      std::cout << result << " (" << value << ")\n";
    }
  }

  std::cout << "\n";
  std::cout << "part 1) sum: " << sum_1 << "\n";
  std::cout << "part 2) sum: " << sum_2 << "\n";
  std::cout << "\n";
}
