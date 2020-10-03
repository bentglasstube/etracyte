#include "camera.h"

double clamp(double value, double min, double max) {
  if (min > value) return min;
  if (max < value) return max;
  return value;
}

Camera::Camera() : xo_(0), yo_(0) {}

void Camera::update(double fx, double fy, const Planet& map, unsigned int elapsed) {
  const double max = kMaxSpeed * elapsed;

  xo_ = clamp(fx - kWidth / 2, xo_ - max, xo_ + max);

  yo_ = clamp(fy - kHeight / 2, yo_ - max, yo_ + max);
  yo_ = clamp(yo_, 0.0, (double)(map.pixel_height() - kHeight));
}

void Camera::snap(double fx, double fy, const Planet& map) {
  xo_ = fx - kWidth;
  yo_ = fy - kHeight;
  yo_ = clamp(yo_, 0.0, (double)(map.pixel_height() - kHeight));
}
