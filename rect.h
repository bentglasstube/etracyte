#pragma once

#include "graphics.h"

class Rect {

  public:
    Rect();
    Rect(double left, double top, double right, double bottom);

    double top, left, right, bottom;
    bool intersect(const Rect& other) const;
    void draw(Graphics& graphics, int xo, int yo, Graphics::Color color, bool filled) const;
};
