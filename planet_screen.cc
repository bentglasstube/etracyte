#include "planet_screen.h"

#include <algorithm>
#include <iostream>

#include "ship_screen.h"

PlanetScreen::PlanetScreen(GameState gs) :
  gs_(gs),
  text_("text-tiny.png", 8, 8),
  hud_("ui.png", 4, 8, 8),
  alerts_("alerts.png", 1, 240, 112),
  planet_(), camera_(), astronaut_(), state_(State::Playing),
  crystals_(0), fuel_(10)
{
  astronaut_.set_position(planet_.pixel_width() / 2, -100);
  camera_.snap(astronaut_, planet_);

  for (int i = 0; i < kEnemies; ++i) {
    spawn_enemy();
  }
}

bool PlanetScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  elapsed = std::min(64u, elapsed);

  if (state_ == State::Playing) {
    hint_timer_ += elapsed;

    if (input.key_held(Input::Button::Down)) {
      astronaut_.duck();
    } else {
      astronaut_.stand();
    }

    if (input.key_held(Input::Button::Left)) {
      astronaut_.move_left();
    } else if (input.key_held(Input::Button::Right)) {
      astronaut_.move_right();
    } else {
      astronaut_.stop();
    }

    if (input.key_pressed(Input::Button::A)) {
      if (astronaut_.grounded()) {
        astronaut_.jump();
      } else if (fuel_ > 0) {
        --fuel_;
        audio.play_random_sample("jetpack.wav", 8);
        astronaut_.jump();
      } else {
        audio.play_random_sample("nope.wav", 8);
      }
    }

    if (input.key_pressed(Input::Button::Start)) {
      audio.play_random_sample("beep.wav", 8);
      state_ = State::Paused;
      computer_timer_ = 0;
      choice_ = 0;
    }

    const Item& item = planet_.take_item(astronaut_.x(), astronaut_.y());
    switch (item.type()) {
      case Item::Type::Crystal:
        audio.play_random_sample("crystal.wav", 2);
        crystals_++;
        break;

      default:
        // do nothing
        break;
    }

    for (auto& enemy : enemies_) {
      enemy.update(audio, planet_, astronaut_, elapsed);
    }

    astronaut_.update(planet_, audio, elapsed);
    camera_.update(astronaut_, planet_, elapsed);

  } else if (state_ == State::Paused) {

    computer_timer_ += elapsed;

    if (input.key_pressed(Input::Button::Start)) {
      if (choice_ == 1) {
        if (crystals_ >= gs_.requirement()) {
          state_ = State::Returning;
          warp_timer_ = 0;
          audio.play_random_sample("beep.wav", 8);
          audio.play_sample("teleport.wav");
        } else {
          audio.play_random_sample("nope.wav", 8);
          state_ = State::Playing;
          hint_timer_ = 0;

          if (gs_.requirement() == 1) {
            hint_ = "The computer said I need to find some crystals.";
          } else {
            hint_ = "I need " + std::to_string(gs_.requirement()) + " crystals before I can head back.";
          }
        }
      } else {
        audio.play_random_sample("beep.wav", 8);
        state_ = State::Playing;
      }
    }

    if (input.key_pressed(Input::Button::Up)) {
      if (choice_ > 0) {
        --choice_;
        audio.play_random_sample("beep.wav", 8);
      }
    }

    if (input.key_pressed(Input::Button::Down)) {
      if (choice_ < 1) {
        ++choice_;
        audio.play_random_sample("beep.wav", 8);
      }
    }

  } else if (state_ == State::Returning) {
    for (auto& enemy : enemies_) {
      enemy.update(audio, planet_, astronaut_, elapsed);
    }
    warp_timer_ += elapsed;
    if (warp_timer_ > 9000) {
      gs_.crystals += crystals_;
      ++gs_.planets;

      return false;
    }
  }

  return true;
}

void PlanetScreen::draw(Graphics& graphics) const {
  int xo = camera_.xoffset();
  const int yo = camera_.yoffset();
  const int pw = planet_.pixel_width();

  planet_.draw(graphics, xo, yo);
  astronaut_.draw(graphics, xo, yo);

  while (xo < 0) xo += pw;
  while (xo >= pw) xo -= pw;
  for (const auto& enemy : enemies_) {
    enemy.draw(graphics, xo, yo);
    enemy.draw(graphics, xo - pw, yo);
  }

  if (state_ == State::Paused) {
    graphics.draw_rect({0, 0}, {graphics.width(), graphics.height()}, 0x00000055, true);
    const int y = std::max(graphics.height() - computer_timer_, 304);

    alerts_.draw(graphics, 2, 136, y);
    text_.draw(graphics, "Explore", 192, y + 24);
    text_.draw(graphics, "Return to ship", 192, y + 32);
    text_.draw(graphics, ">", 184, y + 24 + 8 * choice_);
  }

  if (state_ == State::Returning) {
    const int beam = std::min(255, 255 * warp_timer_ / 8000);
    const Rect a = astronaut_.hitbox();
    const Graphics::Point p1 = {(int)a.left - xo, 0};
    const Graphics::Point p2 = {(int)a.right - xo, (int)a.bottom - yo};
    graphics.draw_rect(p1, p2, 0xffcc3300 | beam, true);

    if (warp_timer_ > 8000) {
      const int fade = 255 * (warp_timer_ - 8000) / 1000;
      graphics.draw_rect({0, 0}, {graphics.width(), graphics.height()}, 0xffffff00 | fade, true);
    }
  }

  graphics.draw_rect({6, 6}, {6 + 2 * fuel_, 10}, 0xd8ff00ff, true);
  hud_.draw(graphics, 0, 4, 4);
  hud_.draw(graphics, 1, 12, 4);
  hud_.draw(graphics, 1, 20, 4);
  hud_.draw(graphics, 2, 28, 4);

  text_.draw(graphics, std::to_string(crystals_), graphics.width() - 12, 4, Text::Alignment::Right);
  hud_.draw(graphics, 3, graphics.width() - 12, 4);

  if (!hint_.empty() && hint_timer_ < 5000) {
    text_.draw(graphics, hint_, graphics.width() / 2, 420, Text::Alignment::Center);
  }
}

Screen* PlanetScreen::next_screen() const {
  return new ShipScreen(gs_);
}

void PlanetScreen::spawn_enemy() {
  auto tile = planet_.get_random_tile(Planet::Tile::Cave);
  while (tile == Planet::Tile::Cave) {
    tile = planet_.get_tile(tile.x, tile.y - 1);
  }

  if (tile == Planet::Tile::Rock) {
    const auto r = tile.rect();
    enemies_.emplace_back((r.left + r.right) / 2, r.bottom + 6);
  }
}
