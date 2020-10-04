#pragma once

#include "character.h"

class Enemy : public Character {
  public:

    enum class Type { Bat };

    Enemy(Type t, double x, double y);

    void update(const Planet& map, Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics, int xo, int yo) const;
    int sprite() const { return 7; }

  private:

    Type type_;
};

