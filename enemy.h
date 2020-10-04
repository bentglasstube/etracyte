#pragma once

#include <random>

#include "character.h"

class Enemy : public Character {
  public:

    enum class Type { Bat };
    enum class State { Waiting, Attacking, Sleeping };

    Enemy(Type t, double x, double y);

    void update(const Planet& map, const Character& target, unsigned int elapsed);
    void draw(Graphics& graphics, int xo, int yo) const;

    Rect boxh() const;
    Rect boxv() const;
    Rect hitbox() const;
    int sprite() const;

  private:

    static constexpr double kHorzVelo = 0.06;
    static constexpr double kSwoopVelo = 0.12;
    static constexpr double kSwoopAccel = -0.0001;
    static constexpr int kSleepTime = 500;

    Type type_;
    State state_;
    int timer_;
    double ay_;

    std::mt19937 rng_;

    bool can_see(const Character& target) const;

    void swoop(const Planet& map, const Character& target);
    void land();

    void move(const Planet& map, unsigned int elapsed);



};

