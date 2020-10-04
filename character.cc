#include "character.h"

#include <cassert>

Character::Character(int width, int height) :
  sprites_("sprites.png", 5, 16, 32),
  facing_(Facing::Right),
  grounded_(true),
  width_(width), height_(height),
  x_(0), y_(0), vx_(0), vy_(0) {}

bool Character::collision(const Rect& other) const {
  return hitbox().intersect(other);
}

void Character::bounceh(const Rect& box, double bounce) {
  x_ = vx_ > 0 ? box.left - width_ / 2 : box.right + width_ / 2;
  vx_ *= -bounce;
}

void Character::bouncev(const Rect& box, double bounce) {
  if (vy_ > 0) {
    y_ = box.top;
    grounded_ = true;
  } else {
    y_ = box.bottom + height_;
  }
  vy_ *= -bounce;
}

void Character::set_position(double x, double y) {
  x_ = x;
  y_ = y;
}

double Character::dist2(const Character& other) const {
  const double dx = other.x() - x();
  const double dy = other.y() - y();

  return dx * dx + dy * dy;
}
