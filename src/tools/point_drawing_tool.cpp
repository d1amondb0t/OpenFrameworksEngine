#include "point_drawing_tool.h"

namespace engine {

PointDrawingTool::PointDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    color.set("Color", ofColor::red);
    radius.set("Radius", 1.0f, 0.0f, 100.0f);
}

void PointDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    std::unique_ptr<Point> point = buildPoint();
    point->draw();
}

void PointDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void PointDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mouseCurrent = {x, y};
    }
}

void PointDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        std::unique_ptr<Point> point = buildPoint();

        canvas->add(std::move(point));
    }
}

ofParameterGroup PointDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", radius, color);
}

std::unique_ptr<Point> PointDrawingTool::buildPoint() {
    return std::make_unique<Point>(mouseCurrent, radius, color);
}

}  // namespace engine
