#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>

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

      // todo
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
 
  struct Elf {
    std::size_t id;
    int total_calories;
  };

  Elf elf_with_max_calories{0,0};

  {
    Timer t_main("calc");
 
    for (std::size_t idx = 0; idx < elves_inventory.size(); idx++) {
      const auto& inventory = elves_inventory[idx];
      auto total_calories = std::accumulate(inventory.begin(), inventory.end(), 0); 
      if(total_calories > elf_with_max_calories.total_calories) {
        elf_with_max_calories.id = idx + 1;
        elf_with_max_calories.total_calories = total_calories;
      }
    }
  }

  std::cout << "Elf most calories: Elf " << elf_with_max_calories.id <<
               ", with " << elf_with_max_calories.total_calories << " total calories.\n";


}
