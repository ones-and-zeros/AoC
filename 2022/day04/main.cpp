#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "timer.h"

using Span = std::pair<int, int>;
using SpanPair = std::pair<Span, Span>;
using SpanPairs = std::vector<SpanPair>;

constexpr bool DoesFullyContain(const Span& span_a, const Span& span_b) {
  return ( span_a.first >= span_b.first &&
           span_a.second <= span_b.second );
}

constexpr bool DoesFullyContainEither(const Span& span_a, const Span& span_b) {
  return ( DoesFullyContain(span_a, span_b) ||
           DoesFullyContain(span_b, span_a) );
}

constexpr bool DoesOverlap(const Span& span_a, const Span& span_b) {
  return  (span_a.first  >= span_b.first &&
           span_a.first  <= span_b.second ) ||
          (span_a.second  >= span_b.first &&
           span_a.second  <= span_b.second );
}

constexpr bool DoesOverlapEither(const Span& span_a, const Span& span_b) {
  return DoesOverlap(span_a, span_b) || DoesOverlap(span_b, span_a);
}

int main()
{
  Timer t_main("main");

  SpanPairs span_pairs;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      std::istringstream iss{line};
      int a, b, c, d;
      char check1, comma, check2;

      // "a-b,c-d"
      iss >> a >> check1 >> b >> comma >> c >> check2 >> d;
      if(check1 != '-' ||
         comma  != ',' ||
         check2 != '-' ) {
        throw std::logic_error("parsing error");
      }
      span_pairs.push_back({Span{a,b}, Span{c,d}});
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& span_pair : span_pairs) {
  //   std::cout << span_pair.first.first << ","
  //             << span_pair.first.second << " "
  //             << span_pair.second.first << ","
  //             << span_pair.second.second << "\n";
  // }

  std::size_t contained = 0;
  std::size_t overlaped = 0;

  {
    Timer t_main("calc");

    for (const auto& span_pair : span_pairs) {
      if(DoesFullyContainEither(span_pair.first, span_pair.second)) {
        contained++;
      }
      if(DoesOverlapEither(span_pair.first, span_pair.second)) {
        overlaped++;
      }
    }
  }

  std::cout << "\n";
  std::cout << "part 1) contained: " << contained << "\n";
  std::cout << "part 2) overlaped: " << overlaped << "\n";
  std::cout << "\n";
}
