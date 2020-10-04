#include "planet_screen.h"

#include <algorithm>
#include <iostream>

PlanetScreen::PlanetScreen() : text_("text.png"), planet_(), camera_(), astronaut_(), crystals_(0) {
  astronaut_.set_position(planet_.pixel_width() / 2, -100);
  camera_.snap(astronaut_, planet_);

  for (int i = 0; i < kEnemies; ++i) {
    spawn_enemy();
  }
}

bool PlanetScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  elapsed = std::min(64u, elapsed);

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
    astronaut_.jump(audio);
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
    enemy.update(planet_, astronaut_, elapsed);
  }

  astronaut_.update(planet_, audio, elapsed);
  camera_.update(astronaut_, planet_, elapsed);

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

  text_.draw(graphics, std::to_string(crystals_), graphics.width(), 0, Text::Alignment::Right);
  text_.draw(graphics, std::to_string((int)astronaut_.x()), 0, 0);
}

Screen* PlanetScreen::next_screen() const {
  return nullptr;
}

void PlanetScreen::spawn_enemy() {
  auto tile = planet_.get_random_tile(Planet::Tile::Cave);
  while (tile == Planet::Tile::Cave) {
    tile = planet_.get_tile(tile.x, tile.y - 1);
  }

  if (tile == Planet::Tile::Rock) {
    const auto r = tile.rect();
    enemies_.emplace_back(Enemy::Type::Bat, (r.left + r.right) / 2, r.bottom + 6);
  }
}
