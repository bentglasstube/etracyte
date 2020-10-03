#include "character.h"

Character::Character(int width, int height) :
  sprites_("sprites.png", 8, 16, 32),
  facing_(Facing::Right),
  grounded_(true),
  width_(width), height_(height),
  x_(0), y_(0), vx_(0), vy_(0) {}

void Character::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, sprite(), drawx() - xo, drawy() - yo, facing_ == Facing::Left, 0, 0, 0);
}

Rect Character::hitbox() const {
  return { x_, y_, x_ + width_, y_ + height_ };
}

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
