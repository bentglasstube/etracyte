#pragma once

#include <random>

#include "backdrop.h"
#include "screen.h"
#include "text.h"

class ShipScreen : public Screen {
  public:

    ShipScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    struct Star {
      double x, y, vx, vy;
      Graphics::Color color;
    };

    Backdrop bridge_;
    Text text_;

    std::vector<Star> stars_;
    std::mt19937 rng_;
    double stretch_;

    void update_stars(unsigned int elapsed);
};
