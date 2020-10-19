#pragma once

#include <array>
#include <random>
#include <vector>

#include "graphics.h"
#include "spritemap.h"

#include "item.h"
#include "rect.h"

class Planet {
  public:

    class Tile {
      public:
        enum Value : uint8_t { Air, Rock, Cave, OOB, Grass };

        Tile() = default;
        constexpr Tile(Value v) : Tile(v, -1, -1) {}
        constexpr Tile(Value v, int x, int y) : value(v), x(x), y(y), sprite(0) {}

        operator Value() const { return value; }
        explicit operator bool() = delete;

        Rect rect() const {
          return Rect(x * kTileSize, y * kTileSize, (x + 1) * kTileSize, (y + 1) * kTileSize);
        }

        bool obstructs() const {
          switch (value) {
            case Rock:
            case OOB:
            case Grass:
              return true;
            default:
              return false;
          }
        }

        double friction() const {
          return 0.0005;
        }

        Value value;
        int x, y, sprite;
    };

    enum class Biome { Rocky };

    Planet(unsigned int seed);
    void generate_lore();
    void generate();

    Item take_item(double x, double y);
    void draw(Graphics& graphics, int xo, int yo) const;

    Tile tile(double x, double y) const;
    int pixel_width() const { return kMapWidth * kTileSize; }
    int pixel_height() const { return kMapHeight * kTileSize; }

    Tile collision(Rect box, double dx, double dy) const;
    Tile get_random_tile(Tile::Value type); // not const, affects rng

    std::string name() const { return name_; }
    Biome biome() const { return biome_; }
    double mass() const { return mass_; }
    double radius() const { return radius_; }
    double gravity() const { return kGravity * mass_ / radius_ / radius_; }

  private:

    static constexpr int kMapHeight = 256;
    static constexpr int kMapWidth = 1024;
    static constexpr int kTileSize = 16;
    static constexpr double kGravity = 24.615e-21;

    SpriteMap sprites_;
    std::mt19937 rng_;
    std::array<Tile, kMapHeight * kMapWidth> tiles_;
    std::vector<Item> items_;

    std::string name_;
    double mass_, radius_;
    Biome biome_;

    void set_tile(int x, int y, Tile::Value v);
    int index(int x, int y) const;
    Tile check_tiles(int x1, int x2, int y1, int y2) const;
    Tile get_tile(int x, int y) const;

    bool wall(int x, int y) const;
    int nearby(int x, int y, Tile::Value v, int dist = 1) const;
    void smooth_caves();
    void smooth_peaks();
    void pick_sprite(int x, int y, int n);

    double surface_x(int x) const { return x / 128.0f; }
    double cave_x(int x) const { return x / 64.0f; }
    double cave_y(int y) const { return y / 16.0f; }
};
