#pragma once

#include "backdrop.h"
#include "screen.h"
#include "text.h"

class TitleScreen : public Screen {
  public:

    TitleScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    Backdrop title_;
    Text text_;
    bool skip_;
    unsigned int blink_timer_;
};
