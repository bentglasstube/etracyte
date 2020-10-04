#pragma once

#include "character.h"

class Player : public Character {
  public:

    Player();

    void update(const Planet& map, Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics, int xo, int yo) const;

    void move_left();
    void move_right();
    void stop();
    void jump(Audio& audio);
    void duck();
    void stand();

    bool ducking() const { return ducking_; }
    bool crouched() const { return grounded() && ducking(); }

  private:

    static constexpr double kGravity = 0.0005;
    static constexpr double kJumpSpeed = 0.3;
    static constexpr double kAccel = 0.002;
    static constexpr double kDampen = 0.75;
    static constexpr double kBonkForce = 0.2;

    static constexpr int kWidth = 16;
    static constexpr int kHalfWidth = kWidth / 2;
    static constexpr int kHeight = 32;

    double ax_;
    int timer_;
    bool ducking_;

#ifndef NDEBUG
    Rect xcol_, ycol_;
#endif

    void updatex(const Planet& map, unsigned int elapsed);
    void updatey(const Planet& map, Audio& audio, unsigned int elapsed);

    Rect boxh() const;
    Rect boxv() const;
    int sprite() const override;

    bool walking() const { return grounded() && ax_ != 0; }
    bool jumping() const { return vy_ < -0.05; }
};
