#pragma once

#include <random>

#include "backdrop.h"
#include "screen.h"
#include "text.h"

class ShipScreen : public Screen {
  public:

    ShipScreen();

    enum class State { Zooming, Warning, Lore, Descend };

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    std::string get_music_track() const override { return "captain.ogg"; }
    Screen* next_screen() const override;

  private:

    struct Star {
      double x, y, vx, vy;
      Graphics::Color color;
    };

    Backdrop bridge_;
    Text text_;
    State state_;

    double stretch_;
    int timer_;

    std::vector<Star> stars_;
    std::mt19937 rng_;

    void update_stars(unsigned int elapsed);
    void transition(State state);
};
