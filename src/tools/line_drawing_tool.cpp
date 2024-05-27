#include "line_drawing_tool.h"

namespace engine {

LineDrawingTool::LineDrawingTool(std::shared_ptr<DrawingCanvas> canvas) : canvas(canvas) {
    width.set("Width", 1.0f, 0.0f, 100.0f);
    color.set("Color", ofColor::red);
}

void LineDrawingTool::draw() {
    if (!isMouseLeftButtonPressed) return;

    std::unique_ptr<Line> line = buildLine();

    line->draw();
}

void LineDrawingTool::onMouseMoved(int x, int y) {
    mouseCurrent = {x, y};
}

void LineDrawingTool::onMousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = true;
        mousePress = {x, y};
        mouseCurrent = {x, y};
    }
}

void LineDrawingTool::onMouseReleased(int x, int y, int button) {
    if (isMouseLeftButtonPressed && button == OF_MOUSE_BUTTON_LEFT) {
        isMouseLeftButtonPressed = false;
        mouseCurrent = {x, y};

        canvas->add(std::move(buildLine()));
    }
}

ofParameterGroup LineDrawingTool::getParameters() {
    return ofParameterGroup("Parameters", color);
}

std::unique_ptr<Line> LineDrawingTool::buildLine() {
    return std::make_unique<Line>(mousePress, mouseCurrent, width, color);
}

}  // namespace engine
