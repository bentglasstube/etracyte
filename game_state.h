#pragma once

#include <random>

#ifndef NDEBUG
#define SEED 8675309
#else
#include "util.h"
#define SEED Util::random_seed()
#endif

struct GameState {
  std::mt19937 rng;
  int planets = 0;
  int planet_seed = 0;
  int crystals = 0;

  static constexpr int kRequired[8] = { 1, 2, 3, 5, 8, 12, 17, 25 };

  GameState() : rng(SEED), planet_seed(rng()) {}

  int requirement() const {
    return planets < 8 ? kRequired[planets] : 25 + 10 * (planets - 7);
  }

  void next_planet() {
    ++planets;
    planet_seed = rng();
  }
};
