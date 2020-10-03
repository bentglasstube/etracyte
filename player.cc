#include "player.h"

Player::Player() : Character(kWidth, kHeight) {}

void Player::update(const Planet& map, unsigned int elapsed) {
  updatex(map, elapsed);
  updatey(map, elapsed);

  vx_ *= kDampen;
}

void Player::draw(Graphics& graphics, int xo, int yo) const {
  Character::draw(graphics, xo, yo);

#ifndef NDEBUG
  boxh().draw(graphics, xo, yo, 0x0000ffff, false);
  boxv().draw(graphics, xo, yo, 0x0000ffff, false);
  xcol_.draw(graphics, xo, yo, 0xff0000ff, false);
  ycol_.draw(graphics, xo, yo, 0x880000ff, false);
#endif
}

void Player::updatex(const Planet& map, unsigned int elapsed) {
  vx_ += ax_ * elapsed;
  Planet::Tile tile = map.collision(boxh(), vx_ * elapsed, 0);
  if (tile.obstructs()) {
#ifndef NDEBUG
    xcol_ = tile.rect();
#endif
    bounceh(tile.rect(), 0);
  } else {
    x_ += vx_ * elapsed;
  }
}

void Player::updatey(const Planet& map, unsigned int elapsed) {
  vy_ += kGravity * elapsed;
  grounded_ = false;

  Planet::Tile tile = map.collision(boxv(), 0, vy_ * elapsed);
  if (tile.obstructs()) {
#ifndef NDEBUG
    ycol_ = tile.rect();
#endif
    bouncev(tile.rect(), 0);
  } else {
    y_ += vy_ * elapsed;
  }
}

Rect Player::boxh() const {
  return Rect(x_ - kHalfWidth, y_ - kHeight + 4, x_ + kHalfWidth, y_ - 4);
}

Rect Player::boxv() const {
  return Rect(x_ - kHalfWidth + 4, y_ - kHeight, x_ + kHalfWidth - 4, y_);
}

int Player::sprite() const {
  // TODO animate
  return 0;
}
