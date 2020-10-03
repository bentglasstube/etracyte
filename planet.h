#pragma once

#include <array>
#include <random>

#include "graphics.h"

class Planet {
  public:

    enum class Tile { Air, Rock, Cave, OOB };

    Planet();

    void draw(Graphics& graphics, int xo, int yo) const;

  private:

    static constexpr size_t kMapHeight = 256;
    static constexpr size_t kMapWidth = 1024;

    std::mt19937 rand_;
    std::array<Tile, kMapHeight * kMapWidth> tiles_;

    void set_tile(int x, int y, Tile t);
    Tile get_tile(int x, int y) const;
};
