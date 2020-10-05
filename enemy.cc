#include "enemy.h"

Enemy::Enemy(double x, double y) :
  Character(8, 6), state_(State::Waiting),
  timer_(0), ay_(0)
{
  set_position(x, y);
  std::random_device dev;
  rng_.seed(dev());
}

void Enemy::update(Audio& audio, const Planet& map, const Character& target, unsigned int elapsed) {
  switch (state_) {
    case State::Waiting:
      if (can_see(target)) {
        audio.play_random_sample("bat.wav", 4);
        swoop(map, target);
      }
      return;

    case State::Attacking:
      timer_ += elapsed;
      move(map, elapsed);
      return;

    case State::Sleeping:
      timer_ += elapsed;
      if (timer_ > kSleepTime) {
        state_ = State::Waiting;
      }

    default:
      // fuck off
      return;
  }
}

void Enemy::draw(Graphics& graphics, int xo, int yo) const {
  if (x() - xo < -32) return;
  if (x() - xo > graphics.width() + 32) return;

#ifndef NDEBUG
  hitbox().draw(graphics, xo, yo, 0x00ffd833, false);
#endif
  sprites_.draw(graphics, sprite(), x() - width_ / 2 - xo, y() - height_ - yo);
}

Rect Enemy::boxh() const {
  return Rect(x_ - width_ / 2, y_ - height_ + 1, x_ + width_ / 2, y_ - 1);
}

Rect Enemy::boxv() const {
  return Rect(x_ - width_ / 2 + 2, y_ - height_, x_ + width_ / 2 - 2, y_);
}

Rect Enemy::hitbox() const {
  return Rect(x_ - width_ / 2, y_ - height_, x_ + width_ / 2, y_);
}
int Enemy::sprite() const {
  switch (state_) {
    case State::Waiting:
    case State::Sleeping:
      return 7;
    case State::Attacking:
      return 5 + (timer_ / 120) % 2;
    default:
      return 0;
  }
}

bool Enemy::can_see(const Character& target) const {
  return target.y() >= y() && dist2(target) < 16000;
}

void Enemy::swoop(const Planet&, const Character& target) {
  state_ = State::Attacking;

  std::uniform_real_distribution<double> amount(0.7, 1.0);

  vx_ = kHorzVelo * amount(rng_) * (target.x() < x() ? -1 : 1);
  vy_ = kSwoopVelo * amount(rng_);
  ay_ = kSwoopAccel;
}

void Enemy::land() {
  timer_ = 0;
  state_ = State::Sleeping;
  vx_ = 0;
  vy_ = 0;
  ay_ = 0;
}

void Enemy::move(const Planet& map, unsigned int elapsed) {
  const Planet::Tile tiley = map.collision(boxv(), 0, vy_ * elapsed);
  if (tiley.obstructs()) {
    if (vy_ < 0) land();
    bouncev(tiley.rect(), 0.1);
  } else {
    y_ += vy_ * elapsed;
    vy_ += ay_ * elapsed;
  }

  const Planet::Tile tilex = map.collision(boxh(), vx_ * elapsed, 0);
  if (tilex.obstructs()) {
  bounceh(tilex.rect(), 0.3);
  } else {
    x_ += vx_ * elapsed;
  }
}
