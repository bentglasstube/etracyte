#include "game.h"

#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Ludum Dare 47";
  config.graphics.width = 256;
  config.graphics.height = 224;
  config.graphics.scale = 3;
  config.graphics.fullscreen = true;

  Game game(config);
  Screen *start = new TitleScreen();

  game.loop(start);

  return 0;
}
