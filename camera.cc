#include "camera.h"

double clamp(double value, double min, double max) {
  if (min > value) return min;
  if (max < value) return max;
  return value;
}

Camera::Camera() : xo_(0), yo_(0) {}

void Camera::update(const Player& f, const Planet& map, unsigned int elapsed) {
  const double max = kMaxSpeed * elapsed;

  const double fx = f.x();
  const double fy = f.y();

  const Rect target = focus();

  // TODO check for wrapping

  if (fx < target.left) {
    xo_ -= std::min(target.left - fx, max);
  } else if (fx > target.right) {
    xo_ += std::min(fx - target.right, max);
  }

  if (fy < target.top) {
    yo_ -= std::min(target.top - fy, max);
  } else if (fy > target.bottom) {
    yo_ += std::min(fy - target.bottom, max);
  }

  xo_ = clamp(xo_, fx - kWidth, fx);
  yo_ = clamp(yo_, fy - kHeight, fy);

  yo_ = clamp(yo_, 0.0, (double)(map.pixel_height() - kHeight));
}

void Camera::snap(const Player& focus, const Planet& map) {
  xo_ = focus.x() - kWidth / 2;
  yo_ = focus.y() - kHeight / 2;
  yo_ = clamp(yo_, 0.0, (double)(map.pixel_height() - kHeight));
}

Rect Camera::focus() const {
  return Rect(
      xo_ + kWidth * (0.5 - kHorzFocus),
      yo_ + kHeight * (0.5 - kVertFocus),
      xo_ + kWidth * (0.5 + kHorzFocus),
      yo_ + kHeight * (0.5 + kVertFocus));
}
