#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>

#include "timer.h"

using Calories = int;
using FoodItems = std::vector<Calories>;
using ElvesInventory = std::vector<FoodItems>;

int main()
{
  Timer t_main("main");

  ElvesInventory elves_inventory;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    FoodItems food_items;

    std::string line;
    while (getline(infile, line)) {
      if (line.empty()) {
        elves_inventory.push_back(food_items);
        food_items.clear();
        continue;
      }

      std::istringstream iss{line};
      std::string calories_s;
      iss >> calories_s;

      // calories
      food_items.push_back(std::stoi(calories_s));
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // for (std::size_t idx = 0; idx < elves_inventory.size(); idx++) {
  //   const auto& inventory = elves_inventory[idx];
  //   auto total_calories = std::accumulate(inventory.begin(), inventory.end(), 0); 
  //   std::cout << "Elf " << (idx + 1) << " = " <<
  //      total_calories << ":\n";
  //   for (const auto food_item : inventory) {
  //     std::cout << "  " << food_item << "\n";   
  //   }
  // }
 
  std::vector<int> calories;
  int calories_top_elf;
  int calories_top_3_elves;

  {
    Timer t_main("calc");
  
    for (std::size_t idx = 0; idx < elves_inventory.size(); idx++) {
      const auto& inventory = elves_inventory[idx];
      calories.push_back(std::accumulate(inventory.begin(), inventory.end(), 0));
    }

    std::sort(calories.begin(), calories.end(), std::greater<int>());
    calories_top_elf = calories[0];
    calories_top_3_elves = std::accumulate(calories.begin(), calories.begin() + 3, 0);
  }

  std::cout << "part-1) Top calories for 1 elf: " << calories_top_elf << "\n";
  std::cout << "part-2) Top calories for 3 elves: " << calories_top_3_elves << "\n";
}
