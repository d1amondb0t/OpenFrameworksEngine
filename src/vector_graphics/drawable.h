#pragma once

#include "ofMain.h"

namespace engine {

class Drawable {
  public:
    virtual void draw() const = 0;
};

}  // namespace engine