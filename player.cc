#include "player.h"

#include <cassert>

#ifndef NDEBUG
#include <sstream>

std::string _str(const double v, int n = 2) {
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << v;
  return out.str();
}
#endif

Player::Player() : Character()
#ifndef NDEBUG
   , text_("text-tiny.png", 8, 8)
#endif
{}

void Player::update(const Planet& map, Audio& audio, unsigned int elapsed) {
  updatex(map, elapsed);
  updatey(map, audio, elapsed);
}

void Player::draw(Graphics& graphics, int xo, int yo) const {
  sprites_.draw_ex(graphics, sprite(), x() - width() / 2 - xo, y() - 32 - yo, facing_ == Facing::Left, 0, 0, 0);

#ifndef NDEBUG
  boxh().draw(graphics, xo, yo, 0x0000ffff, false);
  boxv().draw(graphics, xo, yo, 0x0000ffff, false);
  xcol_.draw(graphics, xo, yo, 0xff0000ff, false);
  ycol_.draw(graphics, xo, yo, 0x880000ff, false);

  text_.draw(graphics, "x:" + _str(x_), 0, 424);
  text_.draw(graphics, _str(vx_), 96, 424);
  text_.draw(graphics, "y:" + _str(y_), 0, 432);
  text_.draw(graphics, _str(vy_), 96, 432);
#endif
}

void Player::move_left() {
  if (grounded()) facing_ = Facing::Left;
  ax_ = -1;
}

void Player::move_right() {
  if (grounded()) facing_ = Facing::Right;
  ax_ = 1;
}

void Player::stop() {
  ax_ = 0;
}

void Player::jump() {
  vy_ = -kJumpSpeed;
}

void Player::cut_jump() {
  vy_ = std::max(-kJumpCutSpeed, vy_);
}

void Player::duck() {
  ducking_ = true;
}

void Player::stand() {
  ducking_ = false;
}

void Player::updatex(const Planet& map, unsigned int elapsed) {
  if (grounded()) {
    double friction = map.tile(x_, y_ + 1).friction();
    vx_ = apply_friction(friction, vx_, elapsed);
    if (!ducking()) vx_ = apply_acceleration(ax_ * kGroundAccel, kMaxSpeed, vx_, elapsed);
  } else {
    vx_ = apply_acceleration(ax_ * kAirAccel, kMaxSpeed, vx_, elapsed);
  }

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
  vy_ = apply_acceleration(map.gravity(), kMaxFall, vy_, elapsed);
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
  return Rect(x_ - width() / 2, y_ - height() + 8, x_ + width() / 2, y_ - 4);
}

Rect Player::boxv() const {
  return Rect(x_ - width() / 2 + 4, y_ - height(), x_ + width() / 2 - 4, y_);
}

int Player::sprite() const {
  if (ducking()) return 4;
  if (jumping()) return 4;
  if (walking()) return (timer_ / 128) % 4;

  return grounded() ? 0 : 3;
}

Rect Player::hitbox() const {
  return Rect(x_ - width() / 2, y_ - height(), x_ + width() / 2, y_);
}
