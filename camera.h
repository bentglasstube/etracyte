#pragma once

#include "planet.h"

class Camera {
  public:

    Camera();

    void update(double fx, double fy, const Planet& map, unsigned int elapsed);
    void snap(double fx, double fy, const Planet& map);

    double xoffset() const { return xo_; }
    double yoffset() const { return yo_; }

  private:

    static constexpr int kWidth = 256;
    static constexpr int kHeight = 224;
    static constexpr double kMaxSpeed = 0.7;

    double xo_, yo_;
};
