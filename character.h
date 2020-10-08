#pragma once

#include "audio.h"
#include "spritemap.h"

#include "planet.h"
#include "rect.h"

class Character {
  public:

    enum class Facing { Left, Right };
    Character();
    virtual ~Character() = default;

    double x() const { return x_; }
    double y() const { return y_; }

    virtual int width() const = 0;
    virtual int height() const = 0;

    bool grounded() const { return grounded_; }

    virtual Rect hitbox() const = 0;
    bool collision(const Rect& other) const;

    void normalize_position(const Planet& map);
    void set_position(double x, double y);
    double dist2(const Character& other) const;

  protected:

    SpriteMap sprites_;
    Facing facing_;

    bool grounded_;
    double x_, y_, vx_, vy_;

    virtual int sprite() const = 0;

    void bounceh(const Rect& box, double bounce = 0.0);
    void bouncev(const Rect& box, double bounce = 0.0);

    static double apply_friction(double friction, double velocity, unsigned int elapsed);
    static double apply_acceleration(double accel, double max, double velocity, unsigned int elapsed);
};
