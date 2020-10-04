#pragma once

#include "audio.h"
#include "spritemap.h"

#include "planet.h"
#include "rect.h"

class Character {
  public:

    enum class Facing { Left, Right };
    Character(int width, int height);

    double x() const { return x_; }
    double y() const { return y_; }

    int width() const { return width_; }
    int height() const { return height_; }

    bool grounded() const { return grounded_; }

    Rect hitbox() const;
    bool collision(const Rect& other) const;

    void set_position(double x, double y);

  protected:

    SpriteMap sprites_;
    Facing facing_;

    bool grounded_;
    int width_, height_;
    double x_, y_, vx_, vy_;

    int drawx() const { return x_ - width_ / 2; }
    int drawy() const { return y_ - height_; }
    virtual int sprite() const = 0;

    void bounceh(const Rect& box);
    void bouncev(const Rect& box);
};
