#pragma once

#include <random>
#include <vector>

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "enemy.h"
#include "game_state.h"
#include "planet.h"
#include "player.h"

class PlanetScreen : public Screen {
  public:

    enum class State { Playing, Paused, Returning };

    PlanetScreen(GameState gs);

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override { return "depths.ogg"; }

  private:

    static constexpr int kEnemies = 2000;
    static constexpr int kStars = 2000;

    struct Star {
      int x, y;
      Graphics::Color color;
      void draw(Graphics& graphics, int xo, int yo) const;
    };

    GameState gs_;
    Text text_;
    SpriteMap hud_, alerts_;
    Planet planet_;
    Camera camera_;
    Player astronaut_;
    State state_;
    std::vector<Enemy> enemies_;
    std::vector<Star> stars_;
    std::string hint_;

    std::mt19937 rng_;

    int crystals_, fuel_;
    int choice_;
    int computer_timer_, hint_timer_, warp_timer_;

    void spawn_enemy();
    void show_hint(const std::string& text);

};
