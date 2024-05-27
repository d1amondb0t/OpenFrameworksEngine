#include "drawing_canvas.h"

namespace engine {

void DrawingCanvas::draw() {
    for (size_t i = 0; i < shapes.size(); i++) {
        shapes[i]->draw();
    }
}

void DrawingCanvas::reset() {
    shapes.clear();
}

void DrawingCanvas::add(std::unique_ptr<Drawable> shape) {
    shapes.push_back(std::move(shape));
}

}  // namespace engine
