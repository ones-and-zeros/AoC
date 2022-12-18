#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include "timer.h"

using Move = std::pair<char, char>;
using StrategyGuide = std::vector<Move>;

Move GetMove(std::istringstream iss) {
  char a, b;
  iss >> a >> b;
  return {a, b};
};

enum class Hand : std:: uint8_t {
  Rock,
  Paper,
  Scissors,
};
bool operator<(Hand a, Hand b) {
  return static_cast<uint8_t>(a) < static_cast<uint8_t>(b);
}

struct Match {
  Hand me;
  Hand other;
};
bool operator<(Match a, Match b) {
  if(a.me == b.me)  {
    return a.other < b.other;    
  }
  return a.me < b.me;
}

enum class Outcome : std:: uint8_t {
  Win,
  Lose,
  Draw,
};
bool operator<(Outcome& a, Outcome& b) {
  return static_cast<uint8_t>(a) < static_cast<uint8_t>(b);
}

std::map<Match, Outcome> match_result_table {
  {{Hand::Rock, Hand::Rock},        Outcome::Draw},
  {{Hand::Rock, Hand::Paper},       Outcome::Lose},
  {{Hand::Rock, Hand::Scissors},    Outcome::Win},
  {{Hand::Paper, Hand::Rock},       Outcome::Win},
  {{Hand::Paper, Hand::Paper},      Outcome::Draw},
  {{Hand::Paper, Hand::Scissors},   Outcome::Lose},
  {{Hand::Scissors, Hand::Rock},    Outcome::Lose},
  {{Hand::Scissors, Hand::Paper},   Outcome::Win},
  {{Hand::Scissors, Hand::Scissors},Outcome::Draw},
};

// The winner of the whole tournament is the player with the highest score.
// Your total score is the sum of your scores for each round.
// The score for a single round is the score for the shape
// you selected plus the outcome

//  shape score:
//  1 for Rock
//  2 for Paper
//  3 for Scissors ...
std::map<Hand, std::uint8_t> hand_value {
  {Hand::Rock, 1},
  {Hand::Paper, 2},
  {Hand::Scissors, 3},
};

//  outcome score:
//  0 if you lost
//  3 if draw
//  6 if you won
std::map<Outcome, std::uint8_t> outcome_value {
  {Outcome::Lose, 0},
  {Outcome::Draw, 3},
  {Outcome::Win, 6},
};

std::uint8_t PlayMatch(Match match) {
  std::uint8_t score = 0;
  return hand_value[match.me] + outcome_value[match_result_table[match]];
}

std::map<char, Hand> strategy_guide_decypher {
  {'A', Hand::Rock},
  {'B', Hand::Paper},
  {'C', Hand::Scissors},
  {'X', Hand::Rock},
  {'Y', Hand::Paper},
  {'Z', Hand::Scissors},
};


int main()
{
  Timer t_main("main");

  StrategyGuide strategy_guide;

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    std::string line;
    while (getline(infile, line)) {
      strategy_guide.push_back(GetMove(std::istringstream{line}));
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // for (const auto& move : strategy_guide) {
  //   std::cout << move.first << " " << move.second << "\n";
  // }

  std::uint64_t total_score = 0;

  {
    Timer t_main("calc");

    for (const auto& move : strategy_guide) {
      const std::uint32_t score = PlayMatch(
        Match{strategy_guide_decypher[move.second],
              strategy_guide_decypher[move.first]});

//      std::cout << score << "\n";

      total_score += score;
    }

  }

  std::cout << "\n";
  std::cout << "part 1) total score: " << total_score << "\n";
  //std::cout << "part 2) Y: " << todo << "\n";
  std::cout << "\n";
}
