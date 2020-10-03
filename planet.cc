#include "planet.h"

#include <assert.h>
#include <algorithm>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

Planet::Planet() {
  std::random_device d;
  rand_.seed(d());

  rand_.seed(8675309);

  std::uniform_int_distribution<int> rs(0, 255);
  const int seed = rs(rand_);

  for (size_t x = 0; x < kMapWidth; ++x) {
    int surface_height = 64 + (int)(64 * stb_perlin_turbulence_noise3(surface_x(x), 0, seed, 2.0f, 0.5f, 6));

    for (size_t y = 0; y < kMapHeight - 1; ++y) {
      if ((int)y > surface_height) {
        float cave = stb_perlin_ridge_noise3(cave_x(x), cave_y(y), seed, 4.0f, 0.5f, 1.0f, 4);
        float threshold = 0.65 - 0.1 * (y - surface_height) / (float)(kMapHeight - surface_height);
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

  smooth_caves();
  smooth_caves();

  for (size_t x = 0; x < kMapWidth; ++x) {
    int n = 0;
    for (size_t y = 0; y < kMapHeight; ++y) {
      Tile t = get_tile(x, y);
      if (t == Tile::Cave) {
        set_tile(x, y, Tile::Air);
        if (++n == 2) break;
      } else if (t == Tile::Rock) {
        break;
      }
    }
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
        default:
          graphics.draw_pixel({x, y}, 0x000000ff);
          break;
      }
    }
  }
}

void Planet::smooth_caves() {
  std::vector<size_t> rocks;
  std::vector<size_t> caves;

  for (size_t y = 0; y < kMapHeight - 1; ++y) {
    for (size_t x = 0; x < kMapWidth; ++x) {
      const Tile t = get_tile(x, y);
      if (t == Tile::Cave || t == Tile::Rock) {
        if (nearby(x, y, Tile::Rock) >= 6) {
          rocks.push_back(x + kMapWidth * y);
        } else {
          caves.push_back(x + kMapWidth * y);
        }
      }
    }
  }

  for (size_t i : rocks) {
    tiles_[i] = Tile::Rock;
  }
  for (size_t i : caves) {
    tiles_[i] = Tile::Cave;
  }
}

int Planet::nearby(int x, int y, Tile t, int dist) const {
  int count = 0;

  for (int ix = x - dist; ix <= x + dist; ++ix) {
    for (int iy = y - dist; iy <= y + dist; ++iy) {
      if (get_tile(ix, iy) == t) ++count;
    }
  }
  return count;
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

bool Planet::wall(int x, int y) const {
  switch (get_tile(x, y)) {
    case Tile::Air:
    case Tile::Cave:
      return false;
    default:
      return true;
      break;
  }
}
