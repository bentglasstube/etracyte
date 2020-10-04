#include "player.h"

#include <cassert>

Player::Player() : Character(kWidth, kHeight) {}

void Player::update(const Planet& map, Audio& audio, unsigned int elapsed) {
  updatex(map, elapsed);
  updatey(map, audio, elapsed);

  vx_ *= kDampen;
}

void Player::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, sprite(), drawx() - xo, drawy() - yo, facing_ == Facing::Left, 0, 0, 0);

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

void Player::jump(Audio& audio) {
  if (!grounded()) audio.play_random_sample("jetpack.wav", 8);
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
    bounceh(tile.rect());
  } else {
    x_ += vx_ * elapsed;
  }

  if (vx_ != 0) timer_ += elapsed;
}

void Player::updatey(const Planet& map, Audio& audio, unsigned int elapsed) {
  vy_ += kGravity * elapsed;
  grounded_ = false;

  Planet::Tile tile = map.collision(boxv(), 0, vy_ * elapsed);
  if (tile.obstructs()) {
#ifndef NDEBUG
    ycol_ = tile.rect();
#endif
    if (std::abs(vy_) > kBonkForce) audio.play_random_sample("bonk.wav", 8);
    bouncev(tile.rect());
  } else {
    y_ += vy_ * elapsed;
  }
}

Rect Player::boxh() const {
  return Rect(x_ - kHalfWidth, y_ - kHeight + 4, x_ + kHalfWidth, y_ - 4);
}

Rect Player::boxv() const {
  return Rect(x_ - kHalfWidth + 4, y_ - kHeight + (jumping() || ducking() ? 4 : 0), x_ + kHalfWidth - 4, y_);
}

int Player::sprite() const {
  if (ducking()) return 4;
  if (jumping()) return 4;
  if (walking()) return (timer_ / 128) % 4;

  return grounded() ? 0 : 3;
}
