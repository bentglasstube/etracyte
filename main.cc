#include "game.h"

#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Ludum Dare 47";
  config.graphics.width = 512;
  config.graphics.height = 448;
  config.graphics.scale = 2;
  config.graphics.fullscreen = false;

  Game game(config);
  Screen *start = new TitleScreen();

  game.loop(start);

  return 0;
}
