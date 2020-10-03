#pragma once

#include <array>
#include <iostream>
#include <random>

#include "graphics.h"
#include "spritemap.h"

#include "rect.h"

class Planet {
  public:

    class Tile {
      public:
        enum Value : uint8_t { Air, Rock, Cave, OOB };

        Tile() = default;
        constexpr Tile(Value v) : Tile(v, -1, -1) {}
        constexpr Tile(Value v, int x, int y) : value_(v), x_(x), y_(y) {}

        operator Value() const { return value_; }
        explicit operator bool() = delete;

        Rect rect() const {
          return Rect(x_ * kTileSize, y_ * kTileSize, (x_ + 1) * kTileSize, (y_ + 1) * kTileSize);
        }

        bool obstructs() const { return value_ == Rock || value_ == OOB; }
        int sprite() const;

      private:
        Value value_;
        int x_, y_;
    };

    Planet();

    void draw(Graphics& graphics, int xo, int yo) const;

    Tile tile(double x, double y) const;
    int pixel_width() const { return kMapWidth * kTileSize; }
    int pixel_height() const { return kMapHeight * kTileSize; }

    Tile collision(Rect box, double dx, double dy) const;

  private:

    static constexpr int kMapHeight = 256;
    static constexpr int kMapWidth = 1024;
    static constexpr int kTileSize = 16;

    SpriteMap sprites_;
    std::mt19937 rand_;
    std::array<Tile, kMapHeight * kMapWidth> tiles_;

    void set_tile(int x, int y, Tile t);
    int index(int x, int y) const;
    Tile get_tile(int x, int y) const;
    Tile check_tiles(int x1, int x2, int y1, int y2) const;

    bool wall(int x, int y) const;
    int nearby(int x, int y, Tile t, int dist = 1) const;
    void smooth_caves();

    float surface_x(int x) const { return x / 128.0f; }
    float cave_x(int x) const { return x / 64.0f; }
    float cave_y(int y) const { return y / 16.0f; }
};
