#include "character.h"

#include <cassert>

Character::Character(int width, int height) :
  sprites_("sprites.png", 5, 16, 32),
  facing_(Facing::Right),
  grounded_(true),
  width_(width), height_(height),
  x_(0), y_(0), vx_(0), vy_(0) {}

void Character::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, sprite(), drawx() - xo, drawy() - yo, facing_ == Facing::Left, 0, 0, 0);
#ifndef NDEBUG
  hitbox().draw(graphics, xo, yo, 0x00ffd8ff, false);
#endif
}

Rect Character::hitbox() const {
  return { x_, y_, x_ + width_, y_ + height_ };
}

bool Character::collision(const Rect& other) const {
  return hitbox().intersect(other);
}

void Character::bounceh(const Rect& box) {
  x_ = vx_ > 0 ? box.left - width_ / 2 : box.right + width_ / 2;
  vx_ = 0;
}

void Character::bouncev(const Rect& box) {
  if (vy_ > 0) {
    y_ = box.top;
    grounded_ = true;
  } else {
    y_ = box.bottom + height_;
  }
  vy_ = 0;
}

void Character::set_position(double x, double y) {
  x_ = x;
  y_ = y;
}
