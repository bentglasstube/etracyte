#include "ship_screen.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

#include "planet_screen.h"
#include "name.h"

ShipScreen::ShipScreen(GameState gs) :
  gs_(gs), bridge_("bridge.png"), text_("text-amber.png"),
  alerts_("alerts.png", 1, 240, 112),
  state_(State::Hyperspeed), stretch_(1000.0), timer_(0)
{
  std::uniform_real_distribution<double> vel(0.04, 0.08);
  std::uniform_real_distribution<double> angle(0.00, 2 * M_PI);
  std::uniform_int_distribution<int> color(200, 255);

  while (stars_.size() < 600) {
    double v = vel(gs_.rng);
    double a = angle(gs_.rng);
    Graphics::Color c = color(gs_.rng) << 24 | color(gs_.rng) << 16 | color(gs_.rng) << 8;
    stars_.push_back({0, 0, v * std::cos(a), v * std::sin(a), c});
  }

  for (int i = 0; i < 10000; ++i) {
    update_stars(1);
  }

  if (gs_.planets == 0) {
    transition(State::Hyperspeed);
    hyperspace_time_ = 3000;
    stretch_ = 1000.0;
  } else {
    transition(State::Preparing);
    generate_welcome_message();
    stretch_ = 0;
  }
}

bool ShipScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  update_stars(elapsed);

  timer_ += elapsed;
  if (state_ == State::Hyperspeed) {
    stretch_ = std::min(1000.0, stretch_ * 1.1);
  } else {
    stretch_ = std::max(1.0, stretch_ * 0.95);
  }

  switch (state_) {
    case State::Preparing:
      if (timer_ > 1000) message_->update(audio, elapsed);
      if (message_->done() && input.any_pressed()) {
        transition(State::Hyperspeed);
        std::uniform_int_distribution<int> hs(3000, 10000);
        hyperspace_time_ = hs(gs_.rng);
      }
      break;

    case State::Hyperspeed:
      if (timer_ > hyperspace_time_) {
        if (gs_.planets == 0) {
          transition(State::Warning);
          audio.play_sample("alert.wav");
          message_.reset(new AppearingText(kAlertText));
        } else {
          transition(State::Lore);
          generate_planet_lore();
        }
      }
      break;

    case State::Warning:
      if (timer_ > 3824) message_->update(audio, elapsed);
      if (message_->done() && input.any_pressed()) {
        transition(State::Descend);
      }
      break;

    case State::Lore:
      if (timer_ > 1000) message_->update(audio, elapsed);
      if (message_->done() && input.any_pressed()) {
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
    case State::Preparing:
    case State::Lore:
      if (timer_ > 1000) {
        alerts_.draw(graphics, 1, 136, 120);
        message_->draw(graphics, text_, 188, 128);
      }
      break;

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
  return new PlanetScreen(gs_);
}

void ShipScreen::generate_welcome_message() {
  message_.reset(new AppearingText(
    "Welcome back!\n"
    "You now have " + std::to_string(gs_.crystals) + "\n"
    "etracyte crystals.\n"
    "Go to a new planet to\n"
    "look for more."));
}

void ShipScreen::generate_planet_lore() {
  Planet planet(gs_.planet_seed);
  planet.generate_lore();

  std::stringstream lore;

  lore << "New Planet Found\n";
  lore << "Name:   " << std::setw(15) << planet.name() << "\n";
  lore << "Radius: " << std::setw(13) << std::setprecision(4) << planet.radius() << "km\n";
  lore << "Mass:   " << std::setw(13) << std::setprecision(3) << planet.mass() << "kg\n";
  lore << "Biome:  " << std::setw(15) << "Caverns" << "\n";
  lore << "Difficulty: " << std::setw(11) << "*" << "\n";

  message_.reset(new AppearingText(lore.str()));
}
