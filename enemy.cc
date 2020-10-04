#include "enemy.h"

Enemy::Enemy(Type t, double x, double y) : Character(8, 6), type_(t) {
  set_position(x, y);
}

void Enemy::update(const Planet&, Audio&, unsigned int) {
  // do nothing
}

/* void Enemy::draw(Graphics& graphics, int xo, int yo) const { */
/* #ifndef NDEBUG */
/*   hitbox().draw(graphics, xo, yo, 0x00ffd8ff, false); */
/* #endif */
/*   sprites_.draw(graphics, sprite(), x_ - xo, y_ - yo); */
/* } */
