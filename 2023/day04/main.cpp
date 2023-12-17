#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "timer.h"

namespace {

struct Card {
  std::vector<int> winning;
  std::vector<int> playing;
};

using Cards = std::vector<Card>;

Cards ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  Cards cards{};
  std::string line;
  while (getline(infile, line)) {
    std::istringstream iss{line};
    std::string title, card_id;
    iss >> title >> card_id;
    if (title != "Card") {
      throw std::logic_error("title error");
    }

    auto card = Card{};
    while (true) {
      char ch{};
      iss >> ch;
      if (ch == '|') {
        break;
      }
      iss.putback(ch);

      int numb{};
      iss >> numb;

      card.winning.push_back(numb);
    }

    while (!iss.eof()) {
      int numb{};
      iss >> numb;
      card.playing.push_back(numb);
    }
    cards.push_back(card);
  }

  infile.close();

  return cards;
}

int Matches(const Card &card) {
  int matches{};
  for (const auto &play : card.playing) {
    for (const auto &win : card.winning) {
      if (play == win) {
        matches++;
      }
    }
  }
  return matches;
}

std::int64_t CalcPart1(const Cards &cards) {
  Timer t_main("calc p1");

  std::int64_t value{};
  for (const auto &card : cards) {
    int matches{Matches(card)};
    if (matches > 0) {
      auto prize = 1U << (matches - 1);

      value += prize;
    }
  }
  return value;
}

std::int64_t CalcPart2(const Cards &cards) {
  Timer t_main("calc p2");

  std::vector<int> card_qtys(cards.size(), 1);

  for (const auto &qty : card_qtys) {
    std::cout << qty << "\n";
  }
  std::cout << "\n";

  std::int64_t value{};
  for (auto pos = 0; pos < cards.size(); pos++) {
    int matches = Matches(cards[pos]);
    for (auto count = 0; count < matches; count++) {
      auto offset = pos + 1 + count;
      if (offset >= card_qtys.size()) {
        break;
      }
      card_qtys[offset] += card_qtys[pos];
    }
  }

  for (const auto &qty : card_qtys) {
    std::cout << qty << "\n";

    value += qty;
  }

  return value;
}

} // namespace

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::logic_error("expected 1 argument for input filepath\n");
  }

  auto cards = ParseInput(argv[1]);

  auto result_p1 = CalcPart1(cards);
  auto result_p2 = CalcPart2(cards);

  std::cout << "\n";
  std::cout << "part 1: " << result_p1 << "\n";
  std::cout << "part 2: " << result_p2 << "\n";
  std::cout << "\n";
}
