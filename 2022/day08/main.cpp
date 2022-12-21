#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

using RowOfTrees = std::string;
using Forest = std::vector<RowOfTrees>;

bool IsVisible(const Forest& forest, const int row, const int col) {
  const int row_qty = forest.size();
  const int col_qty = forest[0].size();
  const auto& tree = forest[row][col];

  for(int row_idx = 0; row_idx <= row; row_idx++) {
    if(row_idx == row) return true;
    if(forest[row_idx][col] >= tree) break;
  }

  for(int row_idx = row_qty - 1; row_idx >= row; row_idx--) {
    if(row_idx == row) return true;
    if(forest[row_idx][col] >= tree) break;
  }

  for(int col_idx = 0; col_idx <= col; col_idx++) {
    if(col_idx == col) return true;
    if(forest[row][col_idx] >= tree) break;
  }

  for(int col_idx = col_qty - 1; col_idx >= col; col_idx--) {
    if(col_idx == col) return true;
    if(forest[row][col_idx] >= tree) break;
  }

  return false;
}

std::size_t CalcPart1(const Forest& forest) {
  // The first and last row, all trees can be seen
  std::size_t total = forest[0].size() * 2;

  for(int row_idx = 1; row_idx + 1 < forest.size(); row_idx++) {
    // The first and last tree **per row** can be seen
    total += 2;
    for(int col_idx = 1; col_idx + 1 < forest.size(); col_idx++) {
      if (IsVisible(forest, row_idx, col_idx)) {
        total++;
      }
    }
  }

  return total;
}

std::size_t ScenicScore(const Forest& forest, const int row, const int col) {
  const int row_qty = forest.size();
  const int col_qty = forest[0].size();
  const auto& tree = forest[row][col];

  std::size_t north_score = 0;
  std::size_t south_score = 0;
  std::size_t east_score = 0;
  std::size_t west_score = 0;

  for(int row_idx = row + 1; row_idx < row_qty; row_idx++) {
    south_score++;
    if(forest[row_idx][col] >= tree) break;
  }

  for(int row_idx = row - 1; row_idx >= 0; row_idx--) {
    north_score++;
    if(forest[row_idx][col] >= tree) break;
  }

  for(int col_idx = col + 1; col_idx < col_qty; col_idx++) {
    west_score++;
    if(forest[row][col_idx] >= tree) break;
  }

  for(int col_idx = col - 1; col_idx >= 0; col_idx--) {
    east_score++;
    if(forest[row][col_idx] >= tree) break;
  }

  return north_score * south_score * east_score * west_score;
}

std::size_t CalcPart2(const Forest& forest) {
  std::size_t max_scenic_score = 0;
  
  for(int row_idx = 0; row_idx < forest.size(); row_idx++) {
    for(int col_idx = 0; col_idx < forest.size(); col_idx++) {
      const auto scenic_score = ScenicScore(forest, row_idx, col_idx);
      if (scenic_score > max_scenic_score) {
        max_scenic_score = scenic_score;
      }
    }
  }

  return max_scenic_score;
}

int main()
{
  Timer t_main("main");

  Forest forest;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      forest.push_back(line);
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& row : forest) {
  //   std::cout << row << "\n";
  // }
  // std::cout << "\n";

  std::size_t result_p1 = 0;
  std::size_t result_p2 = 0;

  {
    Timer t_main("calc");

    result_p1 = CalcPart1(forest);
    result_p2 = CalcPart2(forest);
  }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
