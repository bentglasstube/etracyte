#include "planet_screen.h"

PlanetScreen::PlanetScreen() : text_("text.png"), planet_(), camera_(), fx_(128), fy_(552), scroll_(false) {
  camera_.snap(fx_, fy_, planet_);
}

bool PlanetScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_held(Input::Button::Up)) fy_ -= elapsed / 4;
  if (input.key_held(Input::Button::Down)) fy_ += elapsed / 4;
  if (input.key_held(Input::Button::Left)) fx_ -= elapsed / 4;
  if (input.key_held(Input::Button::Right)) fx_ += elapsed / 4;

  if (input.key_pressed(Input::Button::A)) scroll_ = !scroll_;
  if (scroll_) fx_ += elapsed;

  camera_.update(fx_, fy_, planet_, elapsed * 2);

  return true;
}

void PlanetScreen::draw(Graphics& graphics) const {
  planet_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  text_.draw(graphics, std::to_string(fx_ - 128), 200, 0);
  text_.draw(graphics, std::to_string(fy_ - 112), 200, 16);
  text_.draw(graphics, std::to_string(camera_.xoffset()), 0, 0);
  text_.draw(graphics, std::to_string(camera_.yoffset()), 0, 16);
}

Screen* PlanetScreen::next_screen() const {
  return nullptr;
}
