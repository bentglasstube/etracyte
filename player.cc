#include "player.h"

#include <cassert>

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

void Player::move_left() {
  facing_ = Facing::Left;
  ax_ = -kAccel;
}

void Player::move_right() {
  facing_ = Facing::Right;
  ax_ = kAccel;
}

void Player::stop() {
  ax_ = 0;
}

void Player::jump() {
  vy_ = -kJumpSpeed;
}

void Player::duck() {
  ducking_ = true;
}

void Player::stand() {
  ducking_ = false;
}

void Player::updatex(const Planet& map, unsigned int elapsed) {
  if (!crouched()) vx_ += ax_ * elapsed;

  Planet::Tile tile = map.collision(boxh(), vx_ * elapsed, 0);
  if (tile.obstructs()) {
#ifndef NDEBUG
    xcol_ = tile.rect();
#endif
    bounceh(tile.rect(), 0);
  } else {
    x_ += vx_ * elapsed;
  }

  if (vx_ != 0) timer_ += elapsed;
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
  if (ducking()) return 4;
  if (jumping()) return 4;
  if (walking()) return (timer_ / 128) % 4;

  return grounded() ? 0 : 3;
}
