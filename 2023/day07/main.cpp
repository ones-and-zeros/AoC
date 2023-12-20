#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "timer.h"

namespace {

enum class Card : int {
  kJoker = 0,
  k2,
  k3,
  k4,
  k5,
  k6,
  k7,
  k8,
  k9,
  kT,
  kJ,
  kQ,
  kK,
  kA
};

Card ToCard(char ch) {
  switch (std::tolower(ch)) {
  case '2':
    return Card::k2;
  case '3':
    return Card::k3;
  case '4':
    return Card::k4;
  case '5':
    return Card::k5;
  case '6':
    return Card::k6;
  case '7':
    return Card::k7;
  case '8':
    return Card::k8;
  case '9':
    return Card::k9;
  case 't':
    return Card::kT;
  case 'j':
    return Card::kJ;
  case 'q':
    return Card::kQ;
  case 'k':
    return Card::kK;
  case 'a':
    return Card::kA;
  }
  throw std::logic_error("invalid card char");
}

struct Hand {
  static constexpr int kCardQuantity{5};
  std::array<Card, kCardQuantity> cards;
  std::int64_t wager;
};

enum class HandType {
  kHighCard = 0,
  kOnePair,
  kTwoPair,
  kThreeOfAKind,
  kFullHouse,
  kFourOfAKind,
  kFiveOfAKind,
};

HandType ToHandType(const Hand &hand) {
  std::multiset<Card> cards{};
  for (const auto &card : hand.cards) {
    cards.insert(card);
  }

  std::multiset<int> matches{};
  int joker_qty{0};
  for (auto card_it = cards.begin(); card_it != cards.end();
       card_it = cards.upper_bound(*card_it)) {
    if (*card_it == Card::kJoker) {
      joker_qty = cards.count(*card_it);
      continue;
    }
    matches.insert(cards.count(*card_it));
  }

  if (joker_qty == 5) {
    return HandType::kFiveOfAKind;
  } else if (joker_qty > 0) {
    auto most_matches = *matches.rbegin();
    auto last_it = matches.end();
    last_it--;
    matches.erase(last_it);
    matches.insert(std::min(most_matches + joker_qty, Hand::kCardQuantity));
  }

  if (matches == std::multiset<int>{5}) {
    return HandType::kFiveOfAKind;
  }
  if (matches == std::multiset<int>{1, 4}) {
    return HandType::kFourOfAKind;
  }
  if (matches == std::multiset<int>{2, 3}) {
    return HandType::kFullHouse;
  }
  if (matches == std::multiset<int>{1, 1, 3}) {
    return HandType::kThreeOfAKind;
  }
  if (matches == std::multiset<int>{1, 2, 2}) {
    return HandType::kTwoPair;
  }
  if (matches == std::multiset<int>{1, 1, 1, 2}) {
    return HandType::kOnePair;
  }
  if (matches == std::multiset<int>{1, 1, 1, 1, 1}) {
    return HandType::kHighCard;
  }

  throw std::logic_error("unexpected hand");
}

bool operator<(const Hand &hand_l, const Hand &hand_r) {
  const auto hand_type_l = ToHandType(hand_l);
  const auto hand_type_r = ToHandType(hand_r);

  if (hand_type_l < hand_type_r) {
    return true;
  }
  if (hand_type_r < hand_type_l) {
    return false;
  }

  for (auto pos = 0; pos < Hand::kCardQuantity; pos++) {
    if (hand_l.cards[pos] < hand_r.cards[pos]) {
      return true;
    }
    if (hand_r.cards[pos] < hand_l.cards[pos]) {
      return false;
    }
  }

  return false;
}

using Hands = std::vector<Hand>;
using Input = Hands;

Input ParseInput(const char *file) {
  Timer t_p("parse input");

  std::ifstream infile{file};
  if (!infile.is_open()) {
    throw std::logic_error("unable to open input file");
  }

  std::vector<Hand> hands{};

  std::string line;
  while (getline(infile, line)) {
    std::istringstream iss{line};

    Hand hand{};
    for (auto count = 0; count < Hand::kCardQuantity; count++) {
      char card_char;
      iss >> card_char;
      hand.cards[count] = ToCard(card_char);
    }

    iss >> hand.wager;

    hands.push_back(hand);
  }

  infile.close();

  return hands;
}

std::int64_t CalcPart1(const Hands &hands) {
  Timer t_main("calc p1");

  auto hands_local = hands;
  std::sort(hands_local.begin(), hands_local.end());

  std::int64_t value{};
  int weight = 1;
  for (const auto &hand : hands_local) {
    value += (hand.wager * weight++);
  }

  return value;
}

std::int64_t CalcPart2(const Hands &hands) {
  Timer t_main("calc p2");

  auto hands_local = hands;
  // convert jacks to jokers
  for (auto &hand : hands_local) {
    for (auto &card : hand.cards) {
      if (card == Card::kJ) {
        card = Card::kJoker;
      }
    }
  }
  std::sort(hands_local.begin(), hands_local.end());

  std::int64_t value{};
  int weight = 1;
  for (const auto &hand : hands_local) {
    value += (hand.wager * weight++);
  }

  return value;
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
