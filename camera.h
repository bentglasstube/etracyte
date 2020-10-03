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

  private:

    static constexpr int kWidth = 512;
    static constexpr int kHeight = 448;
    static constexpr double kMaxSpeed = 0.7;

    double xo_, yo_;
};
