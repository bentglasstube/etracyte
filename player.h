#pragma once

#include "character.h"

#ifndef NDEBUG
#include "text.h"
#endif

class Player : public Character {
  public:

    Player();

    void update(const Planet& map, Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics, int xo, int yo) const;

    int width() const override { return 16; };
    int height() const override { return jumping() || ducking() ? 28 : 32; }

    void move_left();
    void move_right();
    void stop();
    void jump();
    void cut_jump();
    void duck();
    void stand();

    bool ducking() const { return ducking_; }
    Rect hitbox() const override;

    void xwarp(double amount) { x_ += amount; }

  private:

    static constexpr double kMaxFall = 0.5;

    static constexpr double kGroundAccel = 0.001;
    static constexpr double kAirAccel = 0.0002;
    static constexpr double kMaxSpeed = 0.14;

    static constexpr double kJumpSpeed = 0.35;
    static constexpr double kJumpCutSpeed = 0.15;
    static constexpr double kBonkForce = 0.2;

    int ax_, timer_;
    bool ducking_;

#ifndef NDEBUG
    Rect xcol_, ycol_;
    Text text_;
#endif

    void updatex(const Planet& map, unsigned int elapsed);
    void updatey(const Planet& map, Audio& audio, unsigned int elapsed);

    Rect boxh() const override;
    Rect boxv() const override;
    int sprite() const override;

    bool walking() const { return grounded() && ax_ != 0; }
    bool jumping() const { return vy_ < -0.05; }
};
