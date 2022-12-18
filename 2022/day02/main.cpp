#include <cstdint>
 #include <iostream>
 #include <fstream>

#include "timer.h"

int main()
{
  Timer t_main("main");


  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      // if (line.empty()) {
      //   elves_inventory.push_back(food_items);
      //   food_items.clear();
      //   continue;
      // }

      // std::istringstream iss{line};
      // std::string calories_s;
      // iss >> calories_s;

      // // calories
      // food_items.push_back(std::stoi(calories_s));
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  {
    Timer t_main("calc");

    // todo
  }

  //std::cout << "part-1) X: " << todo << "\n";
  //std::cout << "part-2) Y: " << todo << "\n";
}
