#include <cstdint>
#include <fstream>
#include <iostream>

#include "timer.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "error, expected 1 argument for inputfile filepath\n";
    return 1;
  }

  std::ifstream infile{argv[1]};
  if (!infile.is_open()) {
    std::cout << "error, unable to open input file\n";
    return 2;
  }

  {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      // ...
    }

    infile.close();
  }
  std::cout << "\n";

  // // input parse check
  // for (...) {
  //   std::cout << ... << "\n";
  // }

  {
    Timer t_main("calc");

    // ...
  }

  std::cout << "\n";
  std::cout << "part 1) : "
            << "TODO"
            << "\n";
  std::cout << "part 2) : "
            << "TODO"
            << "\n";
  std::cout << "\n";
}
