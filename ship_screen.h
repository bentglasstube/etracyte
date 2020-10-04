#pragma once

#include <memory>
#include <random>

#include "backdrop.h"
#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "appearing_text.h"

class ShipScreen : public Screen {
  public:

    ShipScreen();

    enum class State { Zooming, Warning, Lore, Descend };

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    std::string get_music_track() const override { return "captain.ogg"; }
    Screen* next_screen() const override;

  private:

    static constexpr auto kAlertText =
      " - W A R N I N G - \n"
      "Engine fuel levels are\n"
      "dangerously low. Stop\n"
      "at nearest planet. Get\n"
      "etracyte crystals to\n"
      "replenish fuel.\n";

    struct Star {
      double x, y, vx, vy;
      Graphics::Color color;
    };

    Backdrop bridge_;
    SpriteMap alerts_;
    Text text_;
    State state_;

    double stretch_;
    int timer_;

    std::vector<Star> stars_;
    std::unique_ptr<AppearingText> message_;
    std::mt19937 rng_;

    void update_stars(unsigned int elapsed);
    void transition(State state);
};
