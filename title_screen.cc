#include "title_screen.h"

#include "planet_screen.h"

TitleScreen::TitleScreen() : text_("text.png"), blink_timer_(0) {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  blink_timer_ = (blink_timer_ + elapsed) % 1000;

  return !input.any_pressed();
}

void TitleScreen::draw(Graphics& graphics) const {
  if (blink_timer_ < 500) {
    text_.draw(graphics, "Press Any Key", 256, 400, Text::Alignment::Center);
  }
}

Screen* TitleScreen::next_screen() const {
  return new PlanetScreen;
}
