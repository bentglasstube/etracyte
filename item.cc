#include "item.h"

Item::Item(Type t, double x, double y, int variety) :
  sprites_("objects.png", 8, 16, 16),
  type_(t), x_(x), y_(y), variety_(variety) {}

void Item::draw(Graphics& graphics, int xo, int yo) const {
  const Rect r = rect();

#ifndef NDEBUG
  if (collecitble()) r.draw(graphics, xo, yo, 0xd8ff00ff, false);
#endif

  sprites_.draw(graphics, sprite(), r.left - xo, r.top - yo);
}

Rect Item::rect() const {
  return Rect(x_ - 8, y_ - 16, x_ + 8, y_);
}

int Item::sprite() const {
  switch (type_) {
    case Type::Crystal:
      return 0 + variety_;
    case Type::Stalactite:
      return 2 + variety_;
    default:
      return 0;
  }
}
