#include "planet.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

Planet::Planet() : sprites_("terrain.png", 9, 16, 16) {
  std::random_device dev;
  rand_.seed(dev());

  std::uniform_real_distribution<double> rns(0.0, 1.0);
  const double noise_seed = rns(rand_);

  for (int x = 0; x < kMapWidth; ++x) {
    int surface_height = 64 + (int)(64 * stb_perlin_turbulence_noise3(surface_x(x), 0, noise_seed, 2.0f, 0.5f, 6));

    for (int y = 0; y < kMapHeight - 1; ++y) {
      if ((int)y > surface_height) {
        float cave = stb_perlin_ridge_noise3(cave_x(x), cave_y(y), noise_seed, 4.0f, 0.5f, 1.0f, 4);
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

  for (int x = 0; x < kMapWidth; ++x) {
    int n = 0;
    for (int y = 0; y < kMapHeight; ++y) {
      Tile t = get_tile(x, y);
      if (t == Tile::Cave) {
        if (++n < 2) set_tile(x, y, Tile::Air);
      } else if (t == Tile::Rock) {
        if (get_tile(x, y - 1) == Tile::Air) {
          set_tile(x, y, Tile::Grass);
        }
        break;
      }
    }
  }

  std::uniform_int_distribution<int> percent(0, 99);

  for (int y = 0; y < kMapHeight; ++y) {
    const double iy = (y + 1) * kTileSize;

    for (int x = 0; x < kMapWidth; ++x) {
      const double ix = (x + 0.5) * kTileSize;

      if (get_tile(x, y) == Tile::Cave) {
        if (get_tile(x, y - 1) == Tile::Rock) {
          if (percent(rand_) < 10) {
            items_.emplace_back(Item::Type::Stalactite, ix, iy, percent(rand_) % 2);
          }
        } else if (get_tile(x, y + 1) == Tile::Rock) {
          if (percent(rand_) < 2) {
            items_.emplace_back(Item::Type::Crystal, ix, iy, percent(rand_) % 2);
          }
        }
      }
    }
  }
}

int Planet::Tile::sprite() const {
  switch (value) {
    case Planet::Tile::Air:
      return 0 + variety;
    case Planet::Tile::Cave:
      return 18 + variety;
    case Planet::Tile::Rock:
      return 9 + variety;
    case Planet::Tile::Grass:
      return 27 + variety;
    default:
      return 0;
  }
}

Item Planet::take_item(double x, double y) {
  while (x < 0) x += pixel_width();
  while (x >= pixel_width()) x -= pixel_width();

  for (size_t i = 0; i < items_.size(); ++i) {
    const Item item = items_[i];
    if (item.collecitble() && item.rect().contains(x, y)) {
      items_.erase(items_.begin() + i);
      return item;
    }
  }

  return Item::Nothing();
}

void Planet::draw(Graphics& graphics, int xo, int yo) const {
  while (xo < 0) xo += pixel_width();
  while (xo >= pixel_width()) xo -= pixel_width();

  int ty = std::floor(yo / kTileSize);
  int gy = -(yo % kTileSize);
  while (gy < graphics.height()) {
    int tx = std::floor(xo / kTileSize);
    int gx = -(xo % kTileSize);
    while (gx < graphics.width()) {
      sprites_.draw(graphics, get_tile(tx, ty).sprite(), gx, gy);
      ++tx;
      gx += kTileSize;
    }
    ++ty;
    gy += kTileSize;
  }

  // TODO optimization
  // calculate window and draw items within instead of bootleg drawing all items twice
  for (const auto& i : items_) {
    i.draw(graphics, xo, yo);
    i.draw(graphics, xo - pixel_width(), yo);
  }
}

void Planet::smooth_caves() {
  std::vector<int> rocks;
  std::vector<int> caves;

  for (int y = 0; y < kMapHeight - 1; ++y) {
    for (int x = 0; x < kMapWidth; ++x) {
      const Tile t = get_tile(x, y);
      if (t == Tile::Cave || t == Tile::Rock) {
        if (nearby(x, y, Tile::Rock) >= 6) {
          rocks.push_back(index(x, y));
        } else {
          caves.push_back(index(x, y));
        }
      }
    }
  }

  for (int i : rocks) {
    tiles_[i].value = Tile::Rock;
  }
  for (int i : caves) {
    tiles_[i].value = Tile::Cave;
  }
}

int Planet::nearby(int x, int y, Tile::Value v, int dist) const {
  int count = 0;

  for (int ix = x - dist; ix <= x + dist; ++ix) {
    for (int iy = y - dist; iy <= y + dist; ++iy) {
      if (get_tile(ix, iy) == v) ++count;
    }
  }
  return count;
}

void Planet::set_tile(int x, int y, Tile::Value v) {
  assert(x >= 0 && x < kMapWidth);
  assert(y >= 0 && y < kMapHeight);
  std::uniform_int_distribution<int> flavor(0, 3);

  tiles_[index(x, y)] = Tile(v, x, y, flavor(rand_));
}

Planet::Tile Planet::tile(double x, double y) const {
  return get_tile((int)std::floor(x / (double)kTileSize), (int)std::floor(y / (double)kTileSize));
}

int Planet::index(int x, int y) const {
  while (x < 0) x += kMapWidth;
  return (x % kMapWidth) + kMapWidth * y;
}

Planet::Tile Planet::get_tile(int x, int y) const {
  if (y < 0) return Tile(Tile::Air, x, y);
  if (y >= kMapHeight) return Tile(Tile::Rock, x, y);

  Tile t = tiles_[index(x, y)];
  t.x = x;
  return t;
}

Planet::Tile Planet::collision(Rect box, double dx, double dy) const {
  assert(dx == 0 || dy == 0);

  if (dx != 0) {
    const double x = dx + (dx < 0 ? box.left : box.right);
    return check_tiles(x, x, box.top, box.bottom);
  } else if (dy != 0) {
    const int y = dy + (dy < 0 ? box.top : box.bottom);
    return check_tiles(box.left, box.right, y, y);
  } else {
    return Tile::Air;
  }
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

Planet::Tile Planet::check_tiles(int x1, int x2, int y1, int y2) const {
  assert(y1 == y2 || x1 == x2);
  assert(x1 <= x2);
  assert(y1 <= y2);

  for (int y = std::floor(y1); y <= std::ceil(y2); ++y) {
    for (int x = std::floor(x1); x <= std::ceil(x2); ++x) {
      const Tile t = tile(x, y);
      if (t.obstructs()) return t;
    }
  }
  return Tile::Air;
}

Planet::Tile Planet::get_random_tile(Tile::Value v) {
  std::uniform_int_distribution<int> xr(0, kMapWidth - 1);
  std::uniform_int_distribution<int> yr(0, kMapHeight - 1);

  while (true) {
    const Tile t = get_tile(xr(rand_), yr(rand_));
    if (t == v) return t;
  }
}
