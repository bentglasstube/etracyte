#pragma once

#include <random>

#include "util.h"

struct GameState {
  std::mt19937 rng;
  int planets = 0;
  int crystals = 0;

  static constexpr int kRequired[8] = { 1, 2, 3, 5, 8, 12, 17, 25 };

  GameState() : rng(Util::random_seed()) {}

  int requirement() const {
    return planets < 8 ? kRequired[planets] : 25 + 10 * (planets - 7);
  }
};
