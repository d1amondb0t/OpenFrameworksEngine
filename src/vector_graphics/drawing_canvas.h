#pragma once

#include <memory>
#include <vector>

#include "drawable.h"

namespace engine {

class DrawingCanvas {
  public:
    void draw();
    void reset();
    void add(std::unique_ptr<Drawable> shape);

  private:
    std::vector<std::unique_ptr<Drawable>> shapes;
};

}  // namespace engine