#include "planet_screen.h"

#include <algorithm>

PlanetScreen::PlanetScreen() : text_("text.png"), planet_(), camera_(), astronaut_() {
  astronaut_.set_position(0, -100);
  camera_.snap(astronaut_, planet_);
}

bool PlanetScreen::update(const Input& input, Audio&, unsigned int elapsed) {
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
    astronaut_.jump();
  }

  astronaut_.update(planet_, elapsed);
  camera_.update(astronaut_, planet_, elapsed);

  return true;
}

void PlanetScreen::draw(Graphics& graphics) const {
  planet_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  astronaut_.draw(graphics, camera_.xoffset(), camera_.yoffset());

  text_.draw(graphics, std::to_string((int)astronaut_.x()), 0, 0);
  text_.draw(graphics, std::to_string((int)astronaut_.y()), 0, 16);
}

Screen* PlanetScreen::next_screen() const {
  return nullptr;
}
