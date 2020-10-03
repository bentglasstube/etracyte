#include "planet.h"

#include <assert.h>
#include <algorithm>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

Planet::Planet() {
  std::random_device d;
  rand_.seed(d());

  std::uniform_int_distribution<int> rs(0, 255);
  const int seed = rs(rand_);

  for (size_t x = 0; x < kMapWidth; ++x) {
    int surface_height = 64 + (int)(64 * stb_perlin_turbulence_noise3(x / 128.0f, 0, seed, 2.0f, 0.5f, 6));

    for (size_t y = 0; y < kMapHeight - 1; ++y) {
      if ((int)y > surface_height) {
        float cave = stb_perlin_ridge_noise3(x / 63.0f, y / 16.0f, seed, 4.0f, 0.5f, 1.0f, 4);
        float threshold = 0.7 - 0.3 * (y - surface_height) / (float)(kMapHeight - surface_height);
        if (cave > threshold) {
          set_tile(x, y, Tile::Cave);
        } else {
          set_tile(x, y, Tile::Rock);
        }
      } else {
        set_tile(x, y, Tile::Air);
      }
    }

    set_tile(x, kMapHeight - 1, Tile::Rock);
  }
}

void Planet::draw(Graphics& graphics, int xo, int yo) const {
  for (int x = 0; x < graphics.width(); ++x) {
    for (int y = 0; y < graphics.height(); ++y) {
      switch (get_tile(x - xo, y - yo)) {
        case Tile::Air:
          graphics.draw_pixel({x, y}, 0x0000ffff);
          break;
        case Tile::Rock:
          graphics.draw_pixel({x, y}, 0x884400ff);
          break;
        case Tile::Cave:
          graphics.draw_pixel({x, y}, 0x221100ff);
          break;
        case Tile::OOB:
          graphics.draw_pixel({x, y}, 0x000000ff);
          break;
      }
    }
  }
}

void Planet::set_tile(int x, int y, Tile t) {
  assert(x >= 0 && x < (int)kMapWidth);
  assert(y >= 0 && y < (int)kMapHeight);
  tiles_[x + kMapWidth * y] = t;
}

Planet::Tile Planet::get_tile(int x, int y) const {
  if (y < 0 || y >= (int)kMapHeight) return Tile::OOB;
  return tiles_[(x % kMapWidth) + kMapWidth * y];
}
