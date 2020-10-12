#pragma once

#include <memory>
#include <random>

#include "backdrop.h"
#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "appearing_text.h"
#include "game_state.h"

class ShipScreen : public Screen {
  public:

    ShipScreen(GameState s);

    enum class State { Preparing, Hyperspeed, Warning, Lore, Descend };

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

    GameState gs_;
    Backdrop bridge_;
    Text text_;
    SpriteMap alerts_;
    State state_;

    double stretch_;
    int timer_, hyperspace_time_;

    std::vector<Star> stars_;
    std::unique_ptr<AppearingText> message_;

    void update_stars(unsigned int elapsed);
    void transition(State state);

    void generate_welcome_message();
    void generate_planet_lore();
};
