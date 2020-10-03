#include "planet_screen.h"

PlanetScreen::PlanetScreen() : text_("text.png"), planet_(), xo_(128), yo_(0), scroll_(false) {}

bool PlanetScreen::update(const Input& input, Audio&, unsigned int) {
  if (input.key_held(Input::Button::Up)) ++yo_;
  if (input.key_held(Input::Button::Down)) --yo_;
  if (input.key_held(Input::Button::Left)) ++xo_;
  if (input.key_held(Input::Button::Right)) --xo_;

  if (input.key_pressed(Input::Button::A)) scroll_ = !scroll_;
  if (scroll_) xo_ -= 4;

  return true;
}

void PlanetScreen::draw(Graphics& graphics) const {
  planet_.draw(graphics, xo_, yo_);
  text_.draw(graphics, std::to_string(xo_), 0, 208);
}

Screen* PlanetScreen::next_screen() const {
  return nullptr;
}
