#pragma once

#include "spritemap.h"

#include "rect.h"

class Item {
  public:

    enum class Type { Nothing, Crystal, Stalactite };

    Item(Type t, double x, double y, int variety);

    void draw(Graphics& graphics, int xo, int yo) const;
    Rect rect() const;
    bool collecitble() const { return type_ == Type::Crystal; }

    Type type() const { return type_; }

    static Item Nothing() { return Item(Type::Nothing, 0, 0, 0); }

  private:

    SpriteMap sprites_;
    Type type_;

    double x_, y_;
    int variety_;

    int sprite() const;

};
