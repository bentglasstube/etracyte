#pragma once

#include "audio.h"
#include "graphics.h"
#include "text.h"

class AppearingText {
  public:

    AppearingText(const std::string& text);

    void update(Audio&, unsigned int elapsed);
    void draw(Graphics& graphics, const Text& text, int x, int y) const;

    bool done() const { return count_ / kLetterRate > text_.length(); }
    void finish() { count_ = kLetterRate * text_.length(); }

  private:

    static constexpr int kLetterRate = 30;

    size_t count_;
    std::string text_;
};
