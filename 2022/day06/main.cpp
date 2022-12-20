#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "timer.h"

using DataStream = std::string;
using DataStreams = std::vector<DataStream>;

std::size_t CalcPart1(const DataStream& data_stream) {
  static constexpr std::size_t kWindow{4};      
  for (auto begin = data_stream.begin(); begin + kWindow < data_stream.end(); ++begin) {
    const auto end = begin + kWindow;
    std::set<char> char_set;
    std::for_each(begin, end, [&](const char& c){ char_set.insert(c); });
    if (char_set.size() == kWindow)
      return end - data_stream.begin();
  }
  return 0;
}

std::size_t CalcPart2(const DataStream& data_stream) {
  static constexpr std::size_t kWindow{14};      
  for (auto begin = data_stream.begin(); begin + kWindow < data_stream.end(); ++begin) {
    const auto end = begin + kWindow;
    std::set<char> char_set;
    std::for_each(begin, end, [&](const char& c){ char_set.insert(c); });
    if (char_set.size() == kWindow)
      return end - data_stream.begin();
  }
  return 0;
}

int main()
{
  Timer t_main("main");

  DataStreams data_streams;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      data_streams.push_back(line);
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& data_stream : data_streams) {
  //   std::cout << data_stream << "\n";
  // }
  // std::cout << "\n";

  std::vector<std::size_t> result_p1; 
  std::vector<std::size_t> result_p2; 

  {
    Timer t_main("calc");

    for (const auto& data_stream : data_streams) {
      result_p1.push_back(CalcPart1(data_stream));
      result_p2.push_back(CalcPart2(data_stream));
    }

  }

  std::cout << "\n";
  std::cout << "part 1)\n";
  std::for_each(result_p1.begin(), result_p1.end(), [](const auto s){std::cout << " " << s << "\n";});

  std::cout << "\n";
  std::cout << "part 2)\n";
  std::for_each(result_p2.begin(), result_p2.end(), [](const auto s){std::cout << " " << s << "\n";});
  std::cout << "\n";
}
