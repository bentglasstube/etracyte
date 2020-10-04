#include "enemy.h"

Enemy::Enemy(Type t, double x, double y) : Character(8, 6), type_(t) {
  set_position(x, y);
}

void Enemy::update(const Planet&, Audio&, unsigned int) {
  // do nothing
}

void Enemy::draw(Graphics& graphics, int xo, int yo) const {
  if (drawx() - xo < -32) return;
  if (drawx() - xo > graphics.width() + 32) return;

#ifndef NDEBUG
  hitbox().draw(graphics, xo, yo, 0x00ffd8ff, false);
#endif
  sprites_.draw(graphics, sprite(), drawx() - xo, drawy() - yo);
}
