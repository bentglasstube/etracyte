#include "ship_screen.h"

#include <algorithm>
#include <cmath>

#include "planet_screen.h"

ShipScreen::ShipScreen() :
  bridge_("bridge.png"), alerts_("alerts.png", 1, 240, 112), text_("text-amber.png"),
  state_(State::Zooming), stretch_(1000.0), timer_(0)
{
  std::random_device dev;
  rng_.seed(dev());

  std::uniform_real_distribution<double> vel(0.04, 0.08);
  std::uniform_real_distribution<double> angle(0.00, 2 * M_PI);
  std::uniform_int_distribution<int> color(200, 255);

  while (stars_.size() < 600) {
    double v = vel(rng_);
    double a = angle(rng_);
    Graphics::Color c = color(rng_) << 24 | color(rng_) << 16 | color(rng_) << 8;
    stars_.push_back({0, 0, v * std::cos(a), v * std::sin(a), c});
  }

  for (int i = 0; i < 10000; ++i) {
    update_stars(1);
  }
}

bool ShipScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  update_stars(elapsed);

  timer_ += elapsed;
  if (state_ == State::Zooming) {
    stretch_ = std::min(1000.0, stretch_ * 1.1);
  } else {
    stretch_ = std::max(1.0, stretch_ * 0.95);
  }

  switch (state_) {
    case State::Zooming:
      if (timer_ > 3000) {
        transition(State::Warning);
        audio.play_sample("alert.wav");
        message_.reset(new AppearingText(kAlertText));
      }
      break;

    case State::Warning:
      if (timer_ > 3824) message_->update(audio, elapsed);
      if (message_->done() && input.any_pressed()) {
        transition(State::Descend);
      }
      break;

    case State::Lore:
      // TODO show planet information
      if (input.any_pressed()) {
        transition(State::Descend);
      }
      break;

    case State::Descend:
      if (timer_ > 1000) return false;
      break;
  }

  return true;
}

void ShipScreen::update_stars(unsigned int elapsed) {
  for (auto& star : stars_) {
    star.x += star.vx * elapsed;
    star.y += star.vy * elapsed;

    if (std::abs(star.x) > 400) {
      star.x = 0;
      star.y = 0;
    }
  }
}

void ShipScreen::draw(Graphics& graphics) const {
  for (const auto& star : stars_) {
    int b = std::min(255, (int)std::floor((star.x * star.x + 4 * star.y * star.y) / 50));
    int sx1= graphics.width() / 2 + (int)star.x;
    int sy1= graphics.height() / 2 + (int)star.y - 48;
    int sx2 = sx1 + (int)(star.vx * stretch_);
    int sy2 = sy1 + (int)(star.vy * stretch_);

    graphics.draw_line({sx1, sy1}, {sx2, sy2}, star.color | b);
  }

  bridge_.draw(graphics);

  Rect full = {0, 0, (double)graphics.width(), (double)graphics.height()};

  switch (state_) {
    case State::Warning:
      if ((timer_ % 1223) < 725) full.draw(graphics, 0, 0, 0xff000044, true);
      if (timer_ > 3824) {
        alerts_.draw(graphics, 0, 136, 120);
        message_->draw(graphics, text_, 188, 128);
      }
      break;

    case State::Descend:
      full.draw(graphics, 0, 0, (255 * timer_ / 1000), true);
      break;

    default:
      break;
  }
}

void ShipScreen::transition(State state) {
  state_ = state;
  timer_ = 0;
}

Screen* ShipScreen::next_screen() const {
  return new PlanetScreen();
}
