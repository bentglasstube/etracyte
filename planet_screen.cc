#include "planet_screen.h"

#include <algorithm>

PlanetScreen::PlanetScreen() : text_("text.png"), planet_(), camera_(), astronaut_(), crystals_(0) {
  astronaut_.set_position(0, -100);
  camera_.snap(astronaut_, planet_);
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


  astronaut_.update(planet_, elapsed);
  camera_.update(astronaut_, planet_, elapsed);

  return true;
}

void PlanetScreen::draw(Graphics& graphics) const {
  planet_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  astronaut_.draw(graphics, camera_.xoffset(), camera_.yoffset());

  text_.draw(graphics, std::to_string(crystals_), graphics.width(), 0, Text::Alignment::Right);
  text_.draw(graphics, std::to_string((int)astronaut_.x()), 0, 0);
}

Screen* PlanetScreen::next_screen() const {
  return nullptr;
}
