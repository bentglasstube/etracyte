#pragma once

#include <random>

#include "audio.h"

#include "character.h"
#include "planet.h"

class Enemy : public Character {
  public:

    enum class State { Waiting, Attacking, Sleeping };

    Enemy(double x, double y, unsigned int seed);
    ~Enemy() = default;

    void update(Audio& audio, const Planet& map, const Character& target, unsigned int elapsed);
    void draw(Graphics& graphics, int xo, int yo) const;

    int width() const override { return 8; }
    int height() const override { return 6; }

    Rect boxh() const override;
    Rect boxv() const override;
    Rect hitbox() const override;
    int sprite() const override;

  private:

    static constexpr double kHorzVelo = 0.06;
    static constexpr double kSwoopVelo = 0.12;
    static constexpr double kSwoopAccel = -0.0001;
    static constexpr int kSleepTime = 500;

    State state_;
    int timer_;
    double ay_;

    std::mt19937 rng_;

    bool can_see(const Character& target) const;

    void swoop(const Planet& map, const Character& target);
    void land();

    void move(const Planet& map, unsigned int elapsed);



};

