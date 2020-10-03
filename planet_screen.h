#pragma once

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "planet.h"

class PlanetScreen : public Screen {
  public:

    PlanetScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    Text text_;
    Planet planet_;
    Camera camera_;

    double fx_, fy_;
    bool scroll_;
};
