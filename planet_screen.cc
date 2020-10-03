#include "planet_screen.h"

PlanetScreen::PlanetScreen() : text_("text.png"), planet_(), camera_(), astronaut_() {
  astronaut_.set_position(128, 128);
  camera_.snap(astronaut_, planet_);
}

bool PlanetScreen::update(const Input& input, Audio&, unsigned int elapsed) {
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
