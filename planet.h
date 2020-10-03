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

    bool wall(int x, int y) const;
    int nearby(int x, int y, Tile t, int dist = 1) const;
    void smooth_caves();

    float surface_x(size_t x) const { return x / 128.0f; }
    float cave_x(size_t x) const { return x / 64.0f; }
    float cave_y(size_t y) const { return y / 16.0f; }
};
