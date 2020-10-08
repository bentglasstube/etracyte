#include "character.h"

#include <cassert>

Character::Character() :
  sprites_("sprites.png", 5, 16, 32),
  facing_(Facing::Right),
  grounded_(true),
  x_(0), y_(0), vx_(0), vy_(0) {}

bool Character::collision(const Rect& other) const {
  return hitbox().intersect(other);
}

void Character::bounceh(const Rect& box, double bounce) {
  x_ = vx_ > 0 ? box.left - width() / 2 : box.right + width() / 2;
  vx_ *= -bounce;
}

void Character::bouncev(const Rect& box, double bounce) {
  if (vy_ > 0) {
    y_ = box.top;
    grounded_ = true;
  } else {
    y_ = box.bottom + height();
  }
  vy_ *= -bounce;
}

void Character::normalize_position(const Planet& map) {
  const int width = map.pixel_width();
  const int height = map.pixel_height();

  while (x_ < 0) x_ += width;
  while (x_ >= width) x_ -= width;

  while (y_ < 0) y_ += height;
  while (y_ >= height) y_ -= height;
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

double Character::apply_friction(double f, double v, unsigned int t) {
  return v > 0 ? std::max(0.0, v - f * t) : std::min(0.0, v + f * t);
}

double Character::apply_acceleration(double a, double m, double v, unsigned int t) {
  return a > 0 ? std::min(m, v + a * t) : std::max(-m, v + a * t);
}
