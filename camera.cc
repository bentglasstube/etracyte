#include "camera.h"

double clamp(double value, double min, double max) {
  if (min > value) return min;
  if (max < value) return max;
  return value;
}

Camera::Camera() : xo_(0), yo_(0) {}

void Camera::update(const Player& focus, const Planet& map, unsigned int elapsed) {
  const double max = kMaxSpeed * elapsed;

  xo_ = clamp(focus.x() - kWidth / 2, xo_ - max, xo_ + max);

  yo_ = clamp(focus.y() - kHeight / 2, yo_ - max, yo_ + max);
  yo_ = clamp(yo_, 0.0, (double)(map.pixel_height() - kHeight));
}

void Camera::snap(const Player& focus, const Planet& map) {
  xo_ = focus.x() - kWidth;
  yo_ = focus.y() - kHeight;
  yo_ = clamp(yo_, 0.0, (double)(map.pixel_height() - kHeight));
}
