#pragma once

#include "planet.h"
#include "player.h"

class Camera {
  public:

    Camera();

    void update(const Player& focus, const Planet& map, unsigned int elapsed);
    void snap(const Player& focus, const Planet& map);

    double xoffset() const { return xo_; }
    double yoffset() const { return yo_; }

    Rect focus() const;

    void xwarp(double amount) { xo_ += amount; }

  private:

    static constexpr int kWidth = 512;
    static constexpr int kHeight = 448;
    static constexpr double kHorzFocus = 0.167;
    static constexpr double kVertFocus = 0.25;
    static constexpr double kMaxSpeed = 0.4;

    double xo_, yo_;
};
