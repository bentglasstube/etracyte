#include "title_screen.h"

#include "game_state.h"
#include "planet_screen.h"
#include "ship_screen.h"

TitleScreen::TitleScreen() : title_("title.png"),text_("text-white.png"), blink_timer_(0) {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  blink_timer_ = (blink_timer_ + elapsed) % 1000;

  if (input.key_pressed(Input::Button::Start) || input.key_pressed(Input::Button::A)) {
    skip_ = input.key_held(Input::Button::Select);
    return false;
  }

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  title_.draw(graphics);
  if (blink_timer_ < 500) {
    text_.draw(graphics, "Press Any Key", 256, 420, Text::Alignment::Center);
  }
}

Screen* TitleScreen::next_screen() const {
  GameState gs;
  if (skip_) return new PlanetScreen(gs);
  return new ShipScreen(gs);
}
