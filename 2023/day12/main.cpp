#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

namespace {

using Conditions = std::string;

std::int32_t GetRawVariantQuantity(Conditions conditions) {
  std::int32_t variants{1};
  for (char ch : conditions) {
    if (ch == '?') {
      variants <<= 1;
    }
  }
  return variants;
}

Conditions GetVariant(Conditions conditions, std::int32_t varient_index) {
  std::uint32_t bit_mask{1};
  std::string output{};
  for (char ch : conditions) {
    if (ch != '?') {
      output += ch;
      continue;
    }
    output += (varient_index & bit_mask) ? '.' : '#';
    bit_mask <<= 1;
  }
  return output;
}

std::vector<int> GetContiguous(Conditions conditions) {
  std::vector<int> contiguous{};

  int count = 0;
  for (char ch : conditions) {
    if (ch == '#') {
      count++;
      continue;
    }
    assert(ch == '.');
    if (count > 0) {
      contiguous.push_back(count);
      count = 0;
    }
  }
  if (count > 0) {
    contiguous.push_back(count);
  }
  return contiguous;
}

struct ConditionRecord {
  Conditions conditions;
  std::vector<int> contiguous_damaged;
};

using ConditionRecords = std::vector<ConditionRecord>;

ConditionRecords ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  ConditionRecords records{};

  std::string line;
  while (getline(infile, line)) {
    std::istringstream iss{line};

    ConditionRecord record{};

    iss >> record.conditions;

    int contiguous;
    while (iss >> contiguous) {
      record.contiguous_damaged.push_back(contiguous);
      char comma;
      if (!((iss >> comma) && (comma == ','))) {
        break;
      }
    }

    records.push_back(record);
  }

  infile.close();

  return records;
}

std::int64_t CalcPart1(const ConditionRecords &records) {
  Timer t_main("calc p1");

  std::int64_t total_count = 0;

  for (const auto &record : records) {
    const auto combo_qty = GetRawVariantQuantity(record.conditions);
    std::int64_t count = 0;
    for (auto pos = 0; pos < combo_qty; pos++) {
      if (GetContiguous(GetVariant(record.conditions, pos)) ==
          record.contiguous_damaged) {
        count++;
      }
    }
    total_count += count;

    // std::cout << record.conditions << " " << count << "\n";
  }

  return total_count;
}

std::int64_t CalcPart2(const ConditionRecords &records) {
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
